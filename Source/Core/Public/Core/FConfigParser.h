#ifndef __FCONFIG_PARSER_H__
#define __FCONFIG_PARSER_H__ 

#include "FTypes.h"
#include "Core/FMacro.h"

/* "OptionName=Value" is how an option on a config is parsed here are some examples */
/* RenderAPI=Vulkan
   RenderFullscreen=True */

typedef enum EConfigType
{
    E_CONFIG_TYPE_INT,
    E_CONFIG_TYPE_FLOAT,
    E_CONFIG_TYPE_BOOL,
    E_CONFIG_TYPE_STRING
} EConfigType;

typedef struct FConfigOption
{
    char name[ENTERPRISE_NAME_MAX_LENGTH];
    char defaultValue[ENTERPRISE_NAME_MAX_LENGTH];
    EConfigType type;
} FConfigOption;  

typedef struct FConfigParsedOption
{
    char name[ENTERPRISE_NAME_MAX_LENGTH];
    EConfigType type;
    union
    {
        FInt Int;
        float Float;
        bool Bool;
        char String[ENTERPRISE_NAME_MAX_LENGTH];
    } value;
} FConfigParsedOption;

typedef struct FConfigParser
{
    char ConfigFilename[ENTERPRISE_NAME_MAX_LENGTH];
    FInt64 optionsCount;
    FConfigOption* pOptions;
    FInt64 parsedOptionsCount;
    FConfigParsedOption* pParsedOptions;
} FConfigParser;

FEXPORT FConfigParser* FConfigParserCreate(FInt64 optionsCount, FConfigOption* pOptions);
FEXPORT void FConfigParserDestroy(FConfigParser* pConfigParser);
FEXPORT bool FConfigParserParse(FConfigParser* pConfigParser, const char* pConfigFilename);
FEXPORT bool FConfigParserGetOption(FConfigParser* pConfigParser, const char* pOptionName, FConfigParsedOption* pParsedOption);
FEXPORT void FConfigParserReset(FConfigParser* pConfigParser, const char* pConfigFilename);
FEXPORT void FConfigParserResetOption(FConfigParser* pConfigParser, const char* pConfigFilename, const char* pOptionName);

#endif