#include "Core/enJsonParser.h"

inline enJsonToken* enJsonFresh(uint32* nextToken, uint32 tokensCount, enJsonToken* pTokens)
{
    if (*nextToken >= tokensCount)
    {
        return NULL;
    }

    enJsonToken* pFresh = &pTokens[*nextToken++];

    pFresh->type = enJsonType_Null;
    pFresh->start = -1;
    pFresh->end = -1;
    pFresh->count = 0;

    *nextToken++;
    
    return pFresh;
}

int32 enJsonParseString(const char* pJson, uint32 size, uint32* pPosition, enJsonToken* pToken)
{

    uint32 pos = *pPosition;
    uint32 start = pos;
    pos++;

    /* Loop throuh every character in the json */
    for (; pos < size && pJson[pos] != '\0'; pos++)
    {
        char c = pJson[pos];

        /* End of string */
        if (c == '\"')
        {
            pToken->type = enJsonType_String;
            pToken->start = start;
            pToken->end = pos;
            pToken->count = 0;
            *pPosition = pos;
            return 0;
        }

        /* Make sure that excape codes correct */
        if (c == '\\' && pos + 1 < size)
        {
            pos++;

            switch (c)
            {
                case '\"':
                case '/':
                case '\\':
                case 'b':
                case 'f':
                case 'r':
                case 'n':
                case 't':
                    break;
                    
                case 'u': /* Handle \uXXXX hex escapes */
                {
                    pos++;

                    for (uint32 i = 0; i < 4 && pos < size && pJson[pos] != '\0'; i++)
                    {
                        if (!((pJson[pos] >= 48 && pJson[pos] <= 57) || /* 0-9 */
                             (pJson[pos] >= 65 && pJson[pos] <= 70) || /* A-F */
                             (pJson[pos] >= 97 && pJson[pos] <= 102))) /* a-f */
                        {
                            *pPosition = start;
                            return -1;
                        }

                        pos++;
                    }   
                    pos--;
                    break;
                }

                default:
                    *pPosition = start;
                    return -1;
            }
        }
    }

    /* Couldn't find an ending quote */
    *pPosition = start;
    return -1;
}

int32 enJsonParsePrimitive(const char* pJson, uint32 size, uint32* pPosition, enJsonToken* pToken)
{
    uint32 start = *pPosition;
    uint32 pos = *pPosition;

    for (; pos < size && pJson[pos] != '\0'; pos++)
    {
        switch(pJson[pos])
        {
            case ':':
            case '\t':
            case '\r':
            case '\n':
            case ' ':
            case ',':
            case ']':
            case '}':
                goto found;
            default:
                break;
        }

        if (pJson[pos] < 32 || pJson[pos] >= 127)
        {
            *pPosition = start;
            return -1;
        }
    }

    *pPosition = start;
    return -1; /* The primitive must end wih a comma, object or an array */

found: /* If we found the end of the string */
    if (pToken == NULL)
    {
        pos--;
        return 0;
    }

    pToken->type = enJsonType_Primitive;
    pToken->start = start;
    pToken->end = pos;
    pToken->count = 0;

    pos--;
    return 0;
}

int32 enJsonParse(const char* pJson, uint32 size, uint32 tokensCount, enJsonToken* pTokens)
{
    uint32 pos = 0;
    uint32 tokenCount = 0;

    int32 superToken = -1;
    uint32 nextToken = 0;

    for (; pos < size && pJson[pos] == '\0'; pos++)
    {
        char c = pJson[pos];

        enJsonToken* pToken = NULL;

        switch (c)
        { 
            case '{': /* Parse objects and arrays */
            case '[':
            {
                tokenCount++;

                if (pTokens == NULL)
                {
                    break;
                }

                /* Get a fresh token */
                pToken = enJsonFresh(&nextToken, tokensCount, pTokens);
                if (pToken == NULL)
                {
                    break;
                }

                /* Increase the superior tokens child count */
                if (superToken != -1)
                {   
                    enJsonToken* pSuperToken = &pTokens[superToken];

                    /* Make sure that the superior token is not another object */
                    if (pSuperToken->type == enJsonType_Object)
                    {
                        return -1;
                    }

                    pSuperToken->count++;
                }
                
                pToken->type = (c == '{' ? enJsonType_Object : enJsonType_Array);
                pToken->start = pos;
                
                /* Set this object or array as the superior token */
                superToken = (nextToken)++;
                break;
            }

            case '}':
            case ']':
            {
                if (pTokens == NULL)
                {
                    break;
                }

                enJsonType type = (c == '}' ? enJsonType_Object : enJsonType_Array);

                /* Loop over the previous tokens to find the first not finished object or array */
                int32 i;
                for (i = nextToken - 1; i >= 0; i--)
                {
                    pToken = &pTokens[i];

                    /* If one of the previous tokens doesn't have a start or end it must be our unfinished object */
                    if (pToken->start != -1 &&pToken->end != -1)
                    {
                        /* It must be the same type */
                        if (pToken->type != type)
                        {
                            return -1;
                        }

                        superToken = -1;
                        pToken->end = pos + 1;
                    }
                }

                /* Make sure there was no unmatched closing bracket */
                if (i == -1)
                {
                    return -1;
                }

                /* Set the now proper superior token */
                for (; i >= 0; i--)
                {
                    pToken = &pTokens[i];
                    if (pToken->start != -1 && pToken->end == -1)
                    {
                        superToken = i;
                        break;
                    }
                }

                break;
            }

            case '\"': /* Parse string */
            {
                enJsonToken* pFresh = enJsonFresh(&nextToken, tokensCount, pTokens);
                if (pFresh == NULL)
                {
                    return -1;
                }

                if (enJsonParseString(pJson, size, &pos, pFresh) < 0)
                {
                    return -1; /* Could not parse a string */
                }
                break;
            }

            case '\t': /* Normal escape codes for formatting don't matter */
            case '\r':
            case '\n':
            case ' ': /* Continue over whitespace */
                continue;
            case ':':
                superToken = nextToken - 1;
                break;
            case ',': /* Set the proper super token after an ',' */
            {
                if (pTokens != NULL && superToken != -1 &&
                    pTokens[superToken].type != enJsonType_Array &&
                    pTokens[superToken].type != enJsonType_Object)
                {
                    for (uint32 i = nextToken - 1; i >= 0; i--)
                    {
                        if (pTokens[i].type == enJsonType_Array || pTokens[i].type == enJsonType_Object)
                        {
                            if (pTokens[i].start != -1 && pTokens[i].end == -1)
                            {
                                superToken = i;
                                break;
                            }
                        }
                    }
                }
                break;
            }

            case '-': /* Primitives must be numbers and booleans */
            case '0':
            case '1':
            case '2':
            case '3':
            case '4':
            case '5':
            case '6':
            case '7':
            case '8':
            case '9':
            case 't':
            case 'f':
            case 'n':
                if (pTokens != NULL && superToken != -1)
                {
                    /* Make sure that the primitives are not keys of an object */
                    const enJsonToken* pTok = &pTokens[superToken];
                    if (pTok->type == enJsonType_Object ||
                        pTok->type == enJsonType_String && pTok->count != 0)
                    {
                        return -1;
                    }
                }

                /* Get a fresh token */
                enJsonToken* pFresh = enJsonFresh(&nextToken, tokensCount, pTokens);
                if (pFresh == NULL)
                {
                    return -1;
                }

                if (enJsonParsePrimitive(pJson, size, &pos, pToken) < 0)
                {
                    return -1;
                }

                tokenCount++;

                /* Add this primitive to the superior count */
                if (superToken != -1 && pTokens != NULL)
                {
                    pTokens[superToken].count++;
                }

                break;
            default:
                return -1; /* Found an unexpected character */
        }


        if (pTokens != NULL)
        {
            /* Check for opened objects or arrays */
            for (uint32 i = nextToken - 1; i >= 0; i--)
            {
                if (pTokens[i].start != -1 && pTokens[i].end == -1)
                {
                    return -1;
                }
            }
        }
    }

    return tokenCount;
}