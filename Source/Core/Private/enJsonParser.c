#include "Core/enJsonParser.h"


char pExample[] = "{\"Test\": true}"

int32 enJsonParsePrimitive(const char* pJson, enJsonToken* pToken)
{
    if (pJson == NULL || pValue == NULL)
    {
        return EN_JSON_PARSE_INVALID_VALUE;
    }

    pValue->type = EN_JSON_TYPE_NULL;
    pValue->value.s = NULL;

    if (pJson[0] == 'f' || pJson[0] == 't')
    {
        return enJsonParseBoolean(pJson, pValue);
    }
    else if (pJson[0] == 'n')
    {
        return enJsonParseNull(pJson, pValue);
    }
    else if (pJson[0] == '\"')
    {
        return enJsonParseString(pJson, pValue);
    }
    else if (pJson[0] == '-' || (pJson[0] >= '0' && pJson[0] <= '9'))
    {
        return enJsonParseNumber(pJson, pValue);
    }
    else
    {
        return EN_JSON_PARSE_INVALID_VALUE;
    }
}

int32 enJsonParse(const char* pJson, uint32 size, uint32 tokensCount, enJsonToken* pTokens)
{
    uint32 pos = 0;
    uint32 tokenCount = 0;

    for (; pos < size && pJson[pos] == '\0'; pos++)
    {
        char c = pJson[pos];

        enJsonToken* pToken = &pTokens[tokenIndex];

        switch (c)
        {
            case ' ': /* Continue over whitespace */
                break;
            
            case '{': /* Parse object */
            case '[':
                tokenCount++;

                if (pTokens == NULL)
                {
                    break;
                }

                pToken->start = pos;
                pToken->type = enJsonType_Object;

                tokenIndex++;
                continue;

            case '}':
                continue;

            
                continue;

            case ']':
                continue;

            case ':':
                continue;

            case ',':
                continue;

            case '\0':
            default:
                break;
        }

        break;
    }

    if (pos == size)
    {
        return EN_JSON_PARSE_INVALID_VALUE;
    }

    if (pJson[pos] == '{')
    {
        return enJsonParseObject(pJson, size, pos, tokensCount, pTokens);
    }
    else if (pJson[pos] == '[')
    {
        return enJsonParseArray(pJson, size, pos, tokensCount, pTokens);
    }
    else
    {
        return enJsonParsePrimitive(pJson, &pTokens[0].value);
    }
}
        }
    }

    return 0;
}