#ifndef __EN_JSON_PARSER_H__
#define __EN_JSON_PARSER_H__


/* This is a strict json parser and formatter
    https://www.json.org/json-en.html */

#include "Core/FTypes.h"

typedef enum
{
    enJsonType_Object,
    enJsonType_Array,
    enJsonType_String,
    enJsonType_Number,
    enJsonType_Boolean,
    enJsonType_Null
} enJsonType;

typedef struct
{
    enJsonType type;
    uint32 start;
    uint32 end;
    uint32 size;
} enJsonToken;

/* Parses pJson into a tokenized array of tokens.
    pJson is a null terminated string.
    pTokens is an array that will be filled with tokens.
    pTokensSize is the size of the array.
    Returns the number of tokens parsed. */
int32 enJsonParse(const char* pJson, uint32 size, uint32 tokensCount, enJsonToken* pTokens);

#endif