#ifndef __ENTERPRISE_JSON_PARSER_H__
#define __ENTERPRISE_JSON_PARSER_H__


/* An incredibly tiny strict json parser based off JSMN
    https://www.json.org/json-en.html */

#include "Core/enTypes.h"

typedef enum
{
    enJsonType_Null = 0,
    enJsonType_Object,
    enJsonType_Array,
    enJsonType_String,
    enJsonType_Primitive /* bool and number */
} enJsonType;

typedef struct
{
    enJsonType type;
    int32 start;
    int32 end;
    uint32 count;
} enJsonToken;

/* Parses pJson into a tokenized array of tokens.
    pJson is a null terminated string.
    pTokens is an array that will be filled with tokens.
    pTokensSize is the size of the array.
    Returns the number of tokens parsed. */
int32 enJsonParse(const char* pJson, uint32 size, uint32 tokensCount, enJsonToken* pTokens);

#endif