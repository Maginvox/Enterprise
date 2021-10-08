/* Copyright © 2021 Caden Miller, All Rights Reserved. */

#include "Core/enFile.h"
#include "Core/enLog.h"
#include "Core/enMemory.h"
#include "Core/enString.h"
#include "Core/enConfigParser.h"

bool FConfigParseLine(const char* pLine, char** ppName, int32* pNameLength, char** ppValue, int32* pValueLength, enConfigType* pType)
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
    enConfigType type;

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
            type = enConfigType_Int;
        }
        else if (enStringConvertToFloat(pValue, valueLength, &valueFloat))
        {
            type = enConfigType_Float;
        }
    }
    else if (enStringConvertToBool(pValue, valueLength, &valueBool))
    {
        type = enConfigType_Bool;
    }
    else
    {
        type = enConfigType_String;
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
enConfigParser* enConfigParserCreate(int64 optionsCount, enConfigOption* pOptions)
{
    if (optionsCount <= 0 || pOptions == NULL)
    {
        return NULL;
    }

    enConfigParser* pConfigParser = enCalloc(1, sizeof(enConfigParser));
    if (pConfigParser == NULL)
    {
        return NULL;
    }

    pConfigParser->pOptions = enCalloc(optionsCount, sizeof(enConfigOption));
    if (pConfigParser->pOptions == NULL)
    {
        return NULL;
    }

    pConfigParser->optionsCount = optionsCount;
    enMemoryCopy(pConfigParser->pOptions, pOptions, optionsCount * sizeof(enConfigOption));

    /* Allocate the parsed options to be the same as the default options */
    pConfigParser->pParsedOptions = enCalloc(optionsCount, sizeof(enConfigParsedOption));
    if (pConfigParser->pParsedOptions == NULL)
    {
        return NULL;
    }
    
    return pConfigParser;
}

/* ====================================================== */
void enConfigParserDestroy(enConfigParser* pConfigParser)
{
    if (pConfigParser != NULL)
    {
        if (pConfigParser->pOptions != NULL)
        {
            enFree(pConfigParser->pOptions);
            pConfigParser->pOptions = NULL;
        }
        if (pConfigParser->pParsedOptions != NULL)
        {
            enFree(pConfigParser->pParsedOptions);
            pConfigParser->pParsedOptions = NULL;
        }
        enFree(pConfigParser);
        pConfigParser = NULL;
    }
}

bool enConfigParserParse(enConfigParser* pConfigParser, const char* pConfigFilename)
{
    if (pConfigParser == NULL || pConfigFilename == NULL)
    {
        return false;
    }

    /* Check if the file exists */
    enFile* pFile = enFileOpenOrCreate(pConfigFilename, "r+");
    if (pFile == NULL)
    {
        return false;
    }

    char pBuffer[FSTRING_MAX_LENGTH] = {0};
    while (enFileReadLine(pFile, pBuffer, FSTRING_MAX_LENGTH))
    {
        char* pName = NULL;
        int32 nameLength = 0;
        char* pValue = NULL;
        int32 valueLength = 0;
        enConfigParsedOption parsedOption = {0};

        if (!FConfigParseLine(pBuffer, &pName, &nameLength, &pValue, &valueLength, &parsedOption.type))
        {
            continue;
        }

        /* Check if the option is in the possible list of opition */
        const enConfigOption* pOption = NULL;
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
        case enConfigType_Int:
            conversionResult = enStringConvertToInt(pValue, valueLength, &parsedOption.value.Int);
            break;
        case enConfigType_Float:
            conversionResult = enStringConvertToFloat(pValue,valueLength, &parsedOption.value.Float);
            break;
        case enConfigType_Bool:
            conversionResult = enStringConvertToBool(pValue, valueLength, &parsedOption.value.Bool);
            break;
        case enConfigType_String:
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
        enMemoryCopy(&pConfigParser->pParsedOptions[pConfigParser->parsedOptionsCount++], &parsedOption, sizeof(enConfigParsedOption));
    }

    enFileClose(pFile);
    return true;
}

bool enConfigParserGetOption(enConfigParser* pConfigParser, const char* pOptionName, enConfigParsedOption* pParsedOption)
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
            enMemoryCopy(pParsedOption, &pConfigParser->pParsedOptions[i], sizeof(enConfigParsedOption));
            return true;
        }
    }

    return false;
}

void enConfigParserReset(enConfigParser* pConfigParser, const char* pConfigFilename)
{
    if (pConfigParser == NULL || pConfigFilename == NULL)
    {
        return;
    }

    enFile* pFile = enFileOpen(pConfigFilename, "w");
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

        if (!enFileWrite(pFile, pBuffer, sizeof(pBuffer), 1))
        {
            continue;
        }
    }
}

void enConfigParserResetOption(enConfigParser* pConfigParser, const char* pConfigFilename, const char* pOptionName)
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
    enFile* pFile = enFileOpen(pConfigFilename, "r+");
    if (pFile == NULL)
    {
        return;
    }

    char pBuffer[FSTRING_MAX_LENGTH] = {0};
    while(enFileReadLine(pFile, pBuffer, FSTRING_MAX_LENGTH))
    {

        /* Parse the possible option */
        char* pName = NULL;
        int32 nameLength = 0;
        char* pValue = NULL;
        int32 valueLength = 0;
        enConfigType type;

        if (!FConfigParseLine(pBuffer, &pName, &nameLength, &pValue, &valueLength, &type))
        {
            continue;
        }

        int32 bufferSize = enStringLength(pBuffer, FSTRING_MAX_LENGTH);

        /* Get the names values position */
        int64 valueOffset = (int32)(enFileTell(pFile) - bufferSize) + (pValue - pBuffer);

        /* Check if the option name matches */
        if (enStringCompare(pOptionName, ENTERPRISE_NAME_MAX_LENGTH, pBuffer, nameLength) == 0)
        {
            /* Set the values to the default value */
            char pTemporaryFilePath[ENTERPRISE_PATH_MAX_LENGTH] = {0};
            enStringCopy(pConfigFilename, ENTERPRISE_PATH_MAX_LENGTH, pTemporaryFilePath, ENTERPRISE_PATH_MAX_LENGTH);
            enStringConcatenate(".temp", sizeof(".temp"), pTemporaryFilePath, ENTERPRISE_PATH_MAX_LENGTH);

            /* Backup the config file */
            enFile* pTemporaryFile = enFileOpen(pTemporaryFilePath, "w");
            if (pTemporaryFile == NULL)
            {
                return;
            }

            /* Copy from the start of the config file to the start of the value */
            enFileSeek(pFile, 0, enFileSeek_Set);

            char c;
            while ((c = enFileReadChar(pFile)) != FEOF && enFileTell(pFile) < valueOffset)
            {
                enFileWriteChar(pTemporaryFile, c);
            }

            /* Write the default config value */
            enFileWrite(pTemporaryFile, pDefaultValue, defaultValueLength, 1);
        
            /* Copy the rest of the file to the new config */
            enFileSeek(pFile, valueLength, enFileSeek_Current);
            while ((c = enFileReadChar(pFile)) != FEOF)
            {
                enFileWriteChar(pTemporaryFile, c);
            }
            
            /* Delete the original config and rename the temporary config */
            enFileClose(pFile);
            enFileClose(pTemporaryFile);

            enFileRemove(pConfigFilename);
            enFileRename(pTemporaryFilePath, pConfigFilename);
            break;
        }
    }
}