#include "Core/FFile.h"
#include "Core/FLog.h"
#include "Core/FMemory.h"
#include "Core/FString.h"
#include "Core/FConfigParser.h"

bool FConfigParseLine(const char* pLine, char** ppName, FInt* pNameLength, char** ppValue, FInt* pValueLength, EConfigType* pType)
{
    if (pLine == NULL)
    {
        return false;
    }

    const FInt maxSearchLength = ENTERPRISE_NAME_MAX_LENGTH + sizeof(" = ") + ENTERPRISE_NAME_MAX_LENGTH + 1;

    char* pName = NULL;
    FInt nameLength = 0;
    char* pValue = NULL;
    FInt valueLength = 0;
    EConfigType type;

    char* pLineTemporary = (char*)pLine;

    /* Remove any white spaces */
    while (*pLineTemporary == ' ' && *pLineTemporary != '\0')
    {
        pLineTemporary++;
    }
    
    /* Don't parse comments or invalid lines */
    if (*pLineTemporary == '#' || !FCharacterIsAlpha(*pLineTemporary))
    {
        return false;
    }

    /* Find if there is an "=" symbol */
    char* pEqualsSign = FStringSeperate(pLineTemporary, maxSearchLength, "=", 1);
    if (pEqualsSign == NULL)
    {
        return false;
    }

    /* Find the name */
    pName = pLineTemporary;
    while(FCharacterIsAlpha(*pLineTemporary) && pLineTemporary < pEqualsSign)
    {
        pLineTemporary++;
    }
    nameLength = (FInt)(pLineTemporary - pName);

    if (ppName != NULL)
    {
        *ppName = pName;
    }
    if (pNameLength != NULL)
    {
        *pNameLength = nameLength;
    }

    /* Move past any white spaces and the equals sign */
    while((*pLineTemporary == ' ' || *pLineTemporary == '=') && *pLineTemporary != '\0')
    {
        pLineTemporary++;
    }

    /* Find the value */
    pValue = pLineTemporary;
    while(FCharacterIsAlphaNumeric(*pLineTemporary))
    {
        pLineTemporary++;
    }
    valueLength = (FInt)(pLineTemporary - pValue);

    /* Get the values type */
    FInt valueInteger = 0;
    float valueFloat = 0;
    bool valueBool = false;

    if (FStringIsNumeric(pValue))
    {
        if (FStringConvertToInt(pValue, valueLength, &valueInteger))
        {
            type = E_CONFIG_TYPE_INT;
        }
        else if (FStringConvertToFloat(pValue, valueLength, &valueFloat))
        {
            type = E_CONFIG_TYPE_FLOAT;
        }
    }
    else if (FStringConvertToBool(pValue, valueLength, &valueBool))
    {
        type = E_CONFIG_TYPE_BOOL;
    }
    else
    {
        type = E_CONFIG_TYPE_STRING;
    }

    if (ppValue != NULL)
    {
        *ppValue = pValue;
    }
    if (pValueLength != NULL)
    {
        *pValueLength = valueLength;
    }
    if (pType != NULL)
    {
        *pType = type;
    }
    return true;
}

/* ====================================================== */
FConfigParser* FConfigParserCreate(FInt64 optionsCount, FConfigOption* pOptions)
{
    if (optionsCount <= 0 || pOptions == NULL)
    {
        return NULL;
    }

    FConfigParser* pConfigParser = FAllocateZero(1, sizeof(FConfigParser));
    if (pConfigParser == NULL)
    {
        return NULL;
    }

    pConfigParser->pOptions = FAllocateZero(optionsCount, sizeof(FConfigOption));
    if (pConfigParser->pOptions == NULL)
    {
        return NULL;
    }

    pConfigParser->optionsCount = optionsCount;
    FMemoryCopy(pOptions, pConfigParser->pOptions, optionsCount * sizeof(FConfigOption));

    /* Allocate the parsed options to be the same as the default options */
    pConfigParser->pParsedOptions = FAllocateZero(optionsCount, sizeof(FConfigParsedOption));
    if (pConfigParser->pParsedOptions == NULL)
    {
        return NULL;
    }
    
    return pConfigParser;
}

/* ====================================================== */
void FConfigParserDestroy(FConfigParser* pConfigParser)
{
    if (pConfigParser != NULL)
    {
        if (pConfigParser->pOptions != NULL)
        {
            FDeallocate(pConfigParser->pOptions);
            pConfigParser->pOptions = NULL;
        }
        if (pConfigParser->pParsedOptions != NULL)
        {
            FDeallocate(pConfigParser->pParsedOptions);
            pConfigParser->pParsedOptions = NULL;
        }
        FDeallocate(pConfigParser);
        pConfigParser = NULL;
    }
}

bool FConfigParserParse(FConfigParser* pConfigParser, const char* pConfigFilename)
{
    if (pConfigParser == NULL || pConfigFilename == NULL)
    {
        return false;
    }

    /* Check if the file exists */
    FFile* pFile = FFileOpenOrCreate(pConfigFilename, "r+");
    if (pFile == NULL)
    {
        return false;
    }

    char pBuffer[FSTRING_MAX_LENGTH] = {0};
    while (FFileReadLine(pFile, pBuffer, FSTRING_MAX_LENGTH))
    {
        char* pName = NULL;
        FInt nameLength = 0;
        char* pValue = NULL;
        FInt valueLength = 0;
        FConfigParsedOption parsedOption = {0};

        if (!FConfigParseLine(pBuffer, &pName, &nameLength, &pValue, &valueLength, &parsedOption.type))
        {
            continue;
        }

        /* Check if the option is in the possible list of opition */
        const FConfigOption* pOption = NULL;
        for (FInt64 i = 0; i < pConfigParser->optionsCount; i++)
        {
            if (parsedOption.type == pConfigParser->pOptions[i].type &&
                FStringCompare(pName, nameLength, pConfigParser->pOptions[i].name, ENTERPRISE_NAME_MAX_LENGTH) == 0)
            {
                pOption = &pConfigParser->pOptions[i];
                break;
            }
        }

        if (pOption == NULL)
        {
            continue;
        }

        /* Copy the name */
        FStringCopy(pOption->name, ENTERPRISE_NAME_MAX_LENGTH, parsedOption.name, ENTERPRISE_NAME_MAX_LENGTH);
    
        /* Convert the type */
        bool conversionResult = false;
        switch (parsedOption.type)
        {
        case E_CONFIG_TYPE_INT:
            conversionResult = FStringConvertToInt(pValue, valueLength, &parsedOption.value.Int);
            break;
        case E_CONFIG_TYPE_FLOAT:
            conversionResult = FStringConvertToFloat(pValue,valueLength, &parsedOption.value.Float);
            break;
        case E_CONFIG_TYPE_BOOL:
            conversionResult = FStringConvertToBool(pValue, valueLength, &parsedOption.value.Bool);
            break;
        case E_CONFIG_TYPE_STRING:
            conversionResult = FStringCopy(pValue, valueLength, parsedOption.value.String, ENTERPRISE_NAME_MAX_LENGTH);
            break;
        default:
            conversionResult = false;
            continue;
        }

        if (!conversionResult)
        {
            continue;
        }

        /* Add the parsed option to the parsed options */
        FMemoryCopy(&parsedOption, &pConfigParser->pParsedOptions[pConfigParser->parsedOptionsCount++], sizeof(FConfigParsedOption));
    }

    FFileClose(&pFile);
    return true;
}

bool FConfigParserGetOption(FConfigParser* pConfigParser, const char* pOptionName, FConfigParsedOption* pParsedOption)
{
    if (pConfigParser == NULL || pOptionName == NULL || pParsedOption == NULL)
    {
        return false;
    }

    /* Check if the option exists */
    for (FInt64 i = 0; i < pConfigParser->parsedOptionsCount; i++)
    {
        if (FStringCompare(pOptionName, ENTERPRISE_NAME_MAX_LENGTH, pConfigParser->pParsedOptions[i].name, ENTERPRISE_NAME_MAX_LENGTH) == 0)
        {
            /* Copy the parsed option */
            FMemoryCopy(&pConfigParser->pParsedOptions[i], pParsedOption, sizeof(FConfigParsedOption));
            return true;
        }
    }

    return false;
}

void FConfigParserReset(FConfigParser* pConfigParser, const char* pConfigFilename)
{
    if (pConfigParser == NULL || pConfigFilename == NULL)
    {
        return;
    }

    FFile* pFile = FFileOpen(pConfigFilename, "w");
    if (pFile == NULL)
    {
        return;
    }

    for (FInt64 i = 0; i < pConfigParser->optionsCount; i++)
    {
        const char* pConfigFormat = "%s = %s";
        FStringFormatArgument pConfigFormatArgument[] = 
        {
            {E_STRING_FORMAT_TYPE_STRING, .value.String = pConfigParser->pOptions[i].name},
            {E_STRING_FORMAT_TYPE_STRING, .value.String = pConfigParser->pOptions[i].defaultValue}
        };

        char pBuffer[ENTERPRISE_NAME_MAX_LENGTH + sizeof(" = ") + ENTERPRISE_NAME_MAX_LENGTH];
        if (!FStringFormat(pBuffer, sizeof(pBuffer), pConfigFormat, pConfigFormatArgument, FCOUNT_OF(pConfigFormatArgument)))
        {
            continue; 
        }

        if (!FFileWrite(pFile, pBuffer, sizeof(pBuffer), FStringLength(pBuffer, sizeof(pBuffer)), 1))
        {
            continue;
        }
    }
}

void FConfigParserResetOption(FConfigParser* pConfigParser, const char* pConfigFilename, const char* pOptionName)
{
    if (pConfigParser == NULL || pConfigFilename == NULL || pOptionName == NULL)
    {
        return;
    }

    /* Find the option in the config options */
    const char* pDefaultValue = NULL;
    for (FInt64 i = 0; i < pConfigParser->optionsCount; i++)
    {
        if (FStringCompare(pOptionName, ENTERPRISE_NAME_MAX_LENGTH, pConfigParser->pOptions[i].name, ENTERPRISE_NAME_MAX_LENGTH) == 0)
        {
            /* Get the default value */
            pDefaultValue = pConfigParser->pOptions[i].defaultValue;
            break;
        }
    }

    if (pDefaultValue == NULL)
    {
        return;
    }

    /* Get the default values length */
    FInt64 defaultValueLength = FStringLength(pDefaultValue, ENTERPRISE_NAME_MAX_LENGTH);

    /* Open the config file */
    FFile* pFile = FFileOpen(pConfigFilename, "r+");
    if (pFile == NULL)
    {
        return;
    }

    char pBuffer[FSTRING_MAX_LENGTH] = {0};
    while(FFileReadLine(pFile, pBuffer, FSTRING_MAX_LENGTH))
    {

        /* Parse the possible option */
        char* pName = NULL;
        FInt nameLength = 0;
        char* pValue = NULL;
        FInt valueLength = 0;
        EConfigType type;

        if (!FConfigParseLine(pBuffer, &pName, &nameLength, &pValue, &valueLength, &type))
        {
            continue;
        }

        FInt bufferSize = FStringLength(pBuffer, FSTRING_MAX_LENGTH);

        /* Get the names values position */
        FInt64 valueOffset = (FInt)(FFileTell(pFile) - bufferSize) + (pValue - pBuffer);

        /* Check if the option name matches */
        if (FStringCompare(pOptionName, ENTERPRISE_NAME_MAX_LENGTH, pBuffer, nameLength) == 0)
        {
            /* Set the values to the default value */
            char pTemporaryFilePath[ENTERPRISE_PATH_MAX_LENGTH] = {0};
            FStringCopy(pConfigFilename, ENTERPRISE_PATH_MAX_LENGTH, pTemporaryFilePath, ENTERPRISE_PATH_MAX_LENGTH);
            FStringConcatenate(".temp", sizeof(".temp"), pTemporaryFilePath, ENTERPRISE_PATH_MAX_LENGTH);

            /* Backup the config file */
            FFile* pTemporaryFile = FFileOpen(pTemporaryFilePath, "w");
            if (pTemporaryFile == NULL)
            {
                return;
            }

            /* Copy from the start of the config file to the start of the value */
            FFileSeek(pFile, 0, E_FILE_SEEK_ORIGIN_SET);

            char c;
            while ((c = FFileReadChar(pFile)) != FEOF && FFileTell(pFile) < valueOffset)
            {
                FFileWriteChar(pTemporaryFile, c);
            }

            /* Write the default config value */
            FFileWrite(pTemporaryFile, pDefaultValue, defaultValueLength, defaultValueLength, 1);
        
            /* Copy the rest of the file to the new config */
            FFileSeek(pFile, valueLength, E_FILE_SEEK_ORIGIN_CURRENT);
            while ((c = FFileReadChar(pFile)) != FEOF)
            {
                FFileWriteChar(pTemporaryFile, c);
            }
            
            /* Delete the original config and rename the temporary config */
            FFileClose(&pFile);
            FFileClose(&pTemporaryFile);

            FFileRemove(pConfigFilename);
            FFileRename(pTemporaryFilePath, pConfigFilename);
            break;
        }
    }
}