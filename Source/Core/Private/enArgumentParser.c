/* Copyright © 2021 Caden Miller, All Rights Reserved. */

#include "Core/enLog.h"
#include "Core/enMemory.h"
#include "Core/enString.h"
#include "Core/enArgumentParser.h"

enArgumentParser* enArgumentParserCreate(const int32 optionsCount, const enArgumentOption* pOptions)
{
    if (pOptions == NULL)
    {
        return NULL;
    }

    /* Allocate the parser, and its new options to copy */
    enArgumentParser* pArgsParser = enCalloc(1, sizeof(enArgumentParser));
    if (pArgsParser == NULL)
    {
        return NULL;
    }

    pArgsParser->pOptions = enCalloc(optionsCount, sizeof(enArgumentOption));
    if (pArgsParser->pOptions == NULL)
    {
        enArgumentParserDestroy(pArgsParser);
        return NULL;
    }

    pArgsParser->pParsedOptions = enCalloc(optionsCount, sizeof(enArgumentParsedOption));
    if (pArgsParser->pParsedOptions == NULL)
    {
        enArgumentParserDestroy(pArgsParser);
        return NULL;
    }

    /* Copy the options */
    pArgsParser->optionsCount = optionsCount;
    enMemoryCopy(pArgsParser->pOptions, pOptions, optionsCount * sizeof(enArgumentOption));

    return pArgsParser;
}

void enArgumentParserDestroy(enArgumentParser* pArgumentParser)
{
    if (pArgumentParser == NULL)
    {
        return;
    }

    if (pArgumentParser->pOptions != NULL)
    {
        enFree(pArgumentParser->pOptions);
    }

    enFree(pArgumentParser);
}

bool enArgumentParserParse(enArgumentParser* pArgsParser, const int argc, char** argv)
{
    if (pArgsParser == NULL || argc == 0 || argv == NULL)
    {
        return false;
    }

    for (int i = 0; i < argc; i++)
    {
        char* pArg = (char*)argv[i];

        enArgumentParsedOption parsedOption = {0};
        
        /* Skip the first argument */
        if (i == 0)
        {
            continue;
        }

        /* Skip empty arguments */
        if (pArg[0] == '\0')
        {
            continue;
        }

        int64 argLength = enStringLength(pArg, ENTERPRISE_NAME_MAX_LENGTH);

        /* Make sure that the arguments length is not greater than ENTERPRISE_NAME_MAX_LENGTH */
        if (argLength > ENTERPRISE_NAME_MAX_LENGTH || argLength < 2)
        {
            enLogWarning("An argument was too long!");
            continue;
        }

        /* Find the "--" before the name */
        if (pArg[0] != '-' || pArg[1] != '-')
        {
            enLogWarning("Failed to parse an argument!");
            continue;
        }
        pArg += 2;


        /* Find the "=" */
        char* pEquals = enStringSeperate(pArg, ENTERPRISE_NAME_MAX_LENGTH, "=", sizeof("="));
        if (pEquals == NULL)
        {
            enLogWarning("Could not get an arguments equal sign!");
            continue;
        }

        /* Get the name */
        char* pName = pArg;
        uint32 nameLength = (uint32)(pEquals - pName);

        /* Check the argument name is the same as one of the options */
        enArgumentOption* pOption = NULL;
        for (int j = 0; j < pArgsParser->optionsCount; j++)
        {
            if (enStringCompare(pName, pArgsParser->pOptions[j].name, enStringLength(pArgsParser->pOptions[j].name, ENTERPRISE_NAME_MAX_LENGTH)) == 0)
            {   
                pOption = &pArgsParser->pOptions[j];
                break;
            }
        }

        if (pOption == NULL)
        {
            enLogWarning("An argument was not found in the options!");
            continue;
        }

        /* Set the name in the parsed option */
        enStringCopy(pOption->name, nameLength, parsedOption.name, ENTERPRISE_NAME_MAX_LENGTH);

        /* Check the argument value is not empty */
        if (pArg[0] == '\0')
        {
            enLogWarning("An argument value was empty!");
            continue;
        }

        /* Get the value */
        char* pValue = pEquals + 1;
        int32 valueLength = enStringLength(pValue, ENTERPRISE_NAME_MAX_LENGTH);
        if (valueLength > ENTERPRISE_NAME_MAX_LENGTH)
        {
            enLogWarning("An argument value was too long!");
            continue;
        }

        /* Check the values type */
        int64 valueInteger = 0;
        float valueFloat = 0.0f;
        bool valueBool = false;

        if (enStringIsNumeric(pValue))
        {
            if (enStringConvertToInt(pValue, valueLength, &valueInteger))
            {
                parsedOption.type = enArgumentType_Int;
                parsedOption.value.Int = valueInteger;
            }
            else if (enStringConvertToFloat(pValue, valueLength, &valueFloat))
            {
                parsedOption.type = enArgumentType_Float;
                parsedOption.value.Float = valueFloat;
            }
        }
        else if (enStringConvertToBool(pValue, valueLength, &valueBool))
        {
            parsedOption.type = enArgumentType_Bool;
            parsedOption.value.Bool = valueBool;
        }
        else
        {
            parsedOption.type = enArgumentType_String;
            enStringCopy(pValue, valueLength, parsedOption.value.String, ENTERPRISE_NAME_MAX_LENGTH);
        }

        /* Copy the parsed option */
        enMemoryCopy(&pArgsParser->pParsedOptions[pArgsParser->parsedOptionsCount++], &parsedOption, sizeof(enArgumentParsedOption));
    }

    return true;
}

bool enArgumentParserGetOption(enArgumentParser* pArgsParser, const char* pName, enArgumentParsedOption* pParsedOption)
{
    if (pArgsParser == NULL || pName == NULL || pParsedOption == NULL)
    {
        return false;
    }

    for (int64 i = 0; i < pArgsParser->parsedOptionsCount; i++)
    {
        if (enStringCompare(pName, pArgsParser->pParsedOptions[i].name, enStringLength(pArgsParser->pParsedOptions[i].name, ENTERPRISE_NAME_MAX_LENGTH)) == 0)
        {
            enMemoryCopy(pParsedOption, &pArgsParser->pParsedOptions[i], sizeof(enArgumentParsedOption));
            return true;
        }
    }

    return false;
}