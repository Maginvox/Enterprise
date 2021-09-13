#ifndef __ARGS_PARSER_H__
#define __ARGS_PARSER_H__

/* Arguments are formatted like this "--name=value" to pass a value, or "--name" to indicate an options is on or off */
/* Arguments that do not have this format will be excluded from the retrievable options */

#include "Export.h"
#include "Core/FMacro.h"
#include "Core/FTypes.h"

typedef enum EArgumentType {
    E_ARGUMENT_TYPE_INT,
    E_ARGUMENT_TYPE_FLOAT,
    E_ARGUMENT_TYPE_BOOL,
    E_ARGUMENT_TYPE_STRING,
} EArgumentType;

typedef struct FArgumentOption
{
    char name[ENTERPRISE_NAME_MAX_LENGTH];
    char description[ENTERPRISE_NAME_MAX_LENGTH];
    EArgumentType pType;
} FArgumentOption;

typedef struct FArgumentParsedOption
{
    char name[ENTERPRISE_NAME_MAX_LENGTH];
    EArgumentType type;
    union
    {
        int Int;
        float Float;
        bool Bool;
        char String[ENTERPRISE_NAME_MAX_LENGTH];
    } value;
} FArgumentParsedOption;

typedef struct FArgumentParser
{
    FInt64 optionsCount;
    FArgumentOption* pOptions;
    FInt64 parsedOptionsCount;
    FArgumentParsedOption* pParsedOptions;
} FArgumentParser;

FArgumentParser* FArgumentParserCreate(const FInt32 optionsCount, const FArgumentOption* pOptions);
void FArgumentParserDestroy(FArgumentParser** pArgumentParser);
bool FArgumentParserParse(FArgumentParser* pArgumentParser, const int argc, const char* const* argv);
bool FArgumentParserGetOption(FArgumentParser* pArgumentParser, const char* pName, FArgumentParsedOption* pParsedOption);

#endif