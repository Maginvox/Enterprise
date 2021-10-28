/* Copyright © 2021 Caden Miller, All Rights Reserved. */

#ifndef __ARGS_PARSER_H__
#define __ARGS_PARSER_H__

/* Arguments are formatted like this "--name=value" to pass a value, or "--name" to indicate an options is on or off */
/* Arguments that do not have this format will be excluded from the retrievable options */

#include "Core/enMacro.h"
#include "Core/enTypes.h"

typedef enum enArgumentType
{
    enArgumentType_Int,
    enArgumentType_Float,
    enArgumentType_Bool,
    enArgumentType_String,
} enArgumentType;

typedef struct enArgumentOption
{
    char name[ENTERPRISE_NAME_MAX_LENGTH];
    char description[ENTERPRISE_NAME_MAX_LENGTH];
    enArgumentType pType;
} enArgumentOption;

typedef struct enArgumentParsedOption
{
    char name[ENTERPRISE_NAME_MAX_LENGTH];
    enArgumentType type;
    union
    {
        int64 Int;
        float Float;
        bool Bool;
        char String[ENTERPRISE_NAME_MAX_LENGTH];
    } values;
} enArgumentParsedOption;

typedef struct
{
    int64 optionsCount;
    enArgumentOption* pOptions;
    int64 parsedOptionsCount;
    enArgumentParsedOption* pParsedOptions;
} enArgumentParser;

enArgumentParser* enArgumentParserCreate(const int32 optionsCount, const enArgumentOption* pOptions);
void enArgumentParserDestroy(enArgumentParser* pArgumentParser);
bool enArgumentParserParse(enArgumentParser* pArgumentParser, const int argc, char** argv);
bool enArgumentParserGetOption(enArgumentParser* pArgumentParser, const char* pName, enArgumentParsedOption* pParsedOption);

#endif