/* Copyright © 2021 Caden Miller, All Rights Reserved. */

#include "Core/FFile.h"
#include "Core/FLog.h"
#include "Core/FMemory.h"
#include "Core/FString.h"
#include "Core/FConfigParser.h"

bool FConfigParseLine(const char* pLine, char** ppName, int32* pNameLength, char** ppValue, int32* pValueLength, EConfigType* pType)
{
    if (pLine == NULL)
    {
        return false;
    }

    const int32 maxSearchLength = ENTERPRISE_NAME_MAX_LENGTH + sizeof(" = ") + ENTERPRISE_NAME_MAX_LENGTH + 1;

    char* pName = NULL;
    int32 nameLength = 0;
    char* pValue = NULL;
    int32 valueLength = 0;
    EConfigType type;

    char* pLineTemporary = (char*)pLine;

    /* Remove any white spaces */
    while (*pLineTemporary == ' ' && *pLineTemporary != '\0')
    {
        pLineTemporary++;
    }
    
    /* Don't parse comments or invalid lines */
    if (*pLineTemporary == '#' || !enCharacterIsAlpha(*pLineTemporary))
    {
        return false;
    }

    /* Find if there is an "=" symbol */
    char* pEqualsSign = enStringSeperate(pLineTemporary, maxSearchLength, "=", 1);
    if (pEqualsSign == NULL)
    {
        return false;
    }

    /* Find the name */
    pName = pLineTemporary;
    while(enCharacterIsAlpha(*pLineTemporary) && pLineTemporary < pEqualsSign)
    {
        pLineTemporary++;
    }
    nameLength = (int32)(pLineTemporary - pName);

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
    while(enCharacterIsAlphaNumeric(*pLineTemporary))
    {
        pLineTemporary++;
    }
    valueLength = (int32)(pLineTemporary - pValue);

    /* Get the values type */
    int64 valueInteger = 0;
    float valueFloat = 0;
    bool valueBool = false;

    if (enStringIsNumeric(pValue))
    {
        if (enStringConvertToInt(pValue, valueLength, &valueInteger))
        {
            type = E_CONFIG_TYPE_INT;
        }
        else if (enStringConvertToFloat(pValue, valueLength, &valueFloat))
        {
            type = E_CONFIG_TYPE_FLOAT;
        }
    }
    else if (enStringConvertToBool(pValue, valueLength, &valueBool))
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
FConfigParser* FConfigParserCreate(int64 optionsCount, FConfigOption* pOptions)
{
    if (optionsCount <= 0 || pOptions == NULL)
    {
        return NULL;
    }

    FConfigParser* pConfigParser = enAllocateZero(1, sizeof(FConfigParser));
    if (pConfigParser == NULL)
    {
        return NULL;
    }

    pConfigParser->pOptions = enAllocateZero(optionsCount, sizeof(FConfigOption));
    if (pConfigParser->pOptions == NULL)
    {
        return NULL;
    }

    pConfigParser->optionsCount = optionsCount;
    enMemoryCopy(pOptions, pConfigParser->pOptions, optionsCount * sizeof(FConfigOption));

    /* Allocate the parsed options to be the same as the default options */
    pConfigParser->pParsedOptions = enAllocateZero(optionsCount, sizeof(FConfigParsedOption));
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
            enDeallocate(pConfigParser->pOptions);
            pConfigParser->pOptions = NULL;
        }
        if (pConfigParser->pParsedOptions != NULL)
        {
            enDeallocate(pConfigParser->pParsedOptions);
            pConfigParser->pParsedOptions = NULL;
        }
        enDeallocate(pConfigParser);
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
    enFile* pFile = FFileOpenOrCreate(pConfigFilename, "r+");
    if (pFile == NULL)
    {
        return false;
    }

    char pBuffer[FSTRING_MAX_LENGTH] = {0};
    while (FFileReadLine(pFile, pBuffer, FSTRING_MAX_LENGTH))
    {
        char* pName = NULL;
        int32 nameLength = 0;
        char* pValue = NULL;
        int32 valueLength = 0;
        FConfigParsedOption parsedOption = {0};

        if (!FConfigParseLine(pBuffer, &pName, &nameLength, &pValue, &valueLength, &parsedOption.type))
        {
            continue;
        }

        /* Check if the option is in the possible list of opition */
        const FConfigOption* pOption = NULL;
        for (int64 i = 0; i < pConfigParser->optionsCount; i++)
        {
            if (parsedOption.type == pConfigParser->pOptions[i].type &&
                enStringCompare(pName, nameLength, pConfigParser->pOptions[i].name, ENTERPRISE_NAME_MAX_LENGTH) == 0)
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
        enStringCopy(pOption->name, ENTERPRISE_NAME_MAX_LENGTH, parsedOption.name, ENTERPRISE_NAME_MAX_LENGTH);
    
        /* Convert the type */
        bool conversionResult = false;
        switch (parsedOption.type)
        {
        case E_CONFIG_TYPE_INT:
            conversionResult = enStringConvertToInt(pValue, valueLength, &parsedOption.value.Int);
            break;
        case E_CONFIG_TYPE_FLOAT:
            conversionResult = enStringConvertToFloat(pValue,valueLength, &parsedOption.value.Float);
            break;
        case E_CONFIG_TYPE_BOOL:
            conversionResult = enStringConvertToBool(pValue, valueLength, &parsedOption.value.Bool);
            break;
        case E_CONFIG_TYPE_STRING:
            conversionResult = enStringCopy(pValue, valueLength, parsedOption.value.String, ENTERPRISE_NAME_MAX_LENGTH);
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
        enMemoryCopy(&parsedOption, &pConfigParser->pParsedOptions[pConfigParser->parsedOptionsCount++], sizeof(FConfigParsedOption));
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
    for (int64 i = 0; i < pConfigParser->parsedOptionsCount; i++)
    {
        if (enStringCompare(pOptionName, ENTERPRISE_NAME_MAX_LENGTH, pConfigParser->pParsedOptions[i].name, ENTERPRISE_NAME_MAX_LENGTH) == 0)
        {
            /* Copy the parsed option */
            enMemoryCopy(&pConfigParser->pParsedOptions[i], pParsedOption, sizeof(FConfigParsedOption));
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

    enFile* pFile = FFileOpen(pConfigFilename, "w");
    if (pFile == NULL)
    {
        return;
    }

    for (int64 i = 0; i < pConfigParser->optionsCount; i++)
    {
        const char* pConfigFormat = "%s = %s";
        enStringFormatArgument pConfigFormatArgument[] = 
        {
            {enStringFormatType_String, .value.String = pConfigParser->pOptions[i].name},
            {enStringFormatType_String, .value.String = pConfigParser->pOptions[i].defaultValue}
        };

        char pBuffer[ENTERPRISE_NAME_MAX_LENGTH + sizeof(" = ") + ENTERPRISE_NAME_MAX_LENGTH];
        if (!enStringFormat(pBuffer, sizeof(pBuffer), pConfigFormat, pConfigFormatArgument, COUNT_OF(pConfigFormatArgument)))
        {
            continue; 
        }

        if (!FFileWrite(pFile, pBuffer, sizeof(pBuffer), enStringLength(pBuffer, sizeof(pBuffer)), 1))
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
    for (int64 i = 0; i < pConfigParser->optionsCount; i++)
    {
        if (enStringCompare(pOptionName, ENTERPRISE_NAME_MAX_LENGTH, pConfigParser->pOptions[i].name, ENTERPRISE_NAME_MAX_LENGTH) == 0)
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
    int64 defaultValueLength = enStringLength(pDefaultValue, ENTERPRISE_NAME_MAX_LENGTH);

    /* Open the config file */
    enFile* pFile = FFileOpen(pConfigFilename, "r+");
    if (pFile == NULL)
    {
        return;
    }

    char pBuffer[FSTRING_MAX_LENGTH] = {0};
    while(FFileReadLine(pFile, pBuffer, FSTRING_MAX_LENGTH))
    {

        /* Parse the possible option */
        char* pName = NULL;
        int32 nameLength = 0;
        char* pValue = NULL;
        int32 valueLength = 0;
        EConfigType type;

        if (!FConfigParseLine(pBuffer, &pName, &nameLength, &pValue, &valueLength, &type))
        {
            continue;
        }

        int32 bufferSize = enStringLength(pBuffer, FSTRING_MAX_LENGTH);

        /* Get the names values position */
        int64 valueOffset = (int32)(FFileTell(pFile) - bufferSize) + (pValue - pBuffer);

        /* Check if the option name matches */
        if (enStringCompare(pOptionName, ENTERPRISE_NAME_MAX_LENGTH, pBuffer, nameLength) == 0)
        {
            /* Set the values to the default value */
            char pTemporaryFilePath[ENTERPRISE_PATH_MAX_LENGTH] = {0};
            enStringCopy(pConfigFilename, ENTERPRISE_PATH_MAX_LENGTH, pTemporaryFilePath, ENTERPRISE_PATH_MAX_LENGTH);
            enStringConcatenate(".temp", sizeof(".temp"), pTemporaryFilePath, ENTERPRISE_PATH_MAX_LENGTH);

            /* Backup the config file */
            enFile* pTemporaryFile = FFileOpen(pTemporaryFilePath, "w");
            if (pTemporaryFile == NULL)
            {
                return;
            }

            /* Copy from the start of the config file to the start of the value */
            FFileSeek(pFile, 0, enFileSeek_Set);

            char c;
            while ((c = FFileReadChar(pFile)) != FEOF && FFileTell(pFile) < valueOffset)
            {
                FFileWriteChar(pTemporaryFile, c);
            }

            /* Write the default config value */
            FFileWrite(pTemporaryFile, pDefaultValue, defaultValueLength, defaultValueLength, 1);
        
            /* Copy the rest of the file to the new config */
            FFileSeek(pFile, valueLength, enFileSeek_Current);
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