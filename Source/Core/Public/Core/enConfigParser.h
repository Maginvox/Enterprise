/* Copyright © 2021 Caden Miller, All Rights Reserved. */

#ifndef __FCONFIG_PARSER_H__
#define __FCONFIG_PARSER_H__ 

#include "Core/enTypes.h"
#include "Core/enMacro.h"

/* "OptionName=Value" is how an option on a config is parsed here are some examples */
/* RenderAPI=Vulkan
   RenderFullscreen=True */

typedef enum
{
    enConfigType_Int,
    enConfigType_Float,
    enConfigType_Bool,
    enConfigType_String
} enConfigType;

typedef struct
{
    char name[ENTERPRISE_NAME_MAX_LENGTH];
    char defaultValue[ENTERPRISE_NAME_MAX_LENGTH];
    enConfigType type;
} enConfigOption;  

typedef struct enConfigParsedOption
{
    char name[ENTERPRISE_NAME_MAX_LENGTH];
    enConfigType type;
    union
    {
        int64 Int;
        float Float;
        bool Bool;
        char String[ENTERPRISE_NAME_MAX_LENGTH];
    } value;
} enConfigParsedOption;

typedef struct enConfigParser
{
    char ConfigFilename[ENTERPRISE_NAME_MAX_LENGTH];
    int64 optionsCount;
    enConfigOption* pOptions;
    int64 parsedOptionsCount;
    enConfigParsedOption* pParsedOptions;
} enConfigParser;

enConfigParser* enConfigParserCreate(int64 optionsCount, enConfigOption* pOptions);
void enConfigParserDestroy(enConfigParser* pConfigParser);
bool enConfigParserParse(enConfigParser* pConfigParser, const char* pConfigFilename);
bool enConfigParserGetOption(enConfigParser* pConfigParser, const char* pOptionName, enConfigParsedOption* pParsedOption);
void enConfigParserReset(enConfigParser* pConfigParser, const char* pConfigFilename);
void enConfigParserResetOption(enConfigParser* pConfigParser, const char* pConfigFilename, const char* pOptionName);

#endif