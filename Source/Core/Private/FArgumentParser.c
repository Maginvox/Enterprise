#include "Core/FLog.h"
#include "Core/FMemory.h"
#include "Core/FString.h"
#include "Core/FArgumentParser.h"

FArgumentParser* FArgumentParserCreate(const FInt32 optionsCount, const FArgumentOption* pOptions)
{
    if (pOptions == NULL)
    {
        return NULL;
    }

    /* Allocate the parser, and its new options to copy */
    FArgumentParser* pArgsParser = FAllocateZero(1, sizeof(FArgumentParser));
    if (pArgsParser == NULL)
    {
        return NULL;
    }

    pArgsParser->pOptions = FAllocateZero(optionsCount, sizeof(FArgumentOption));
    if (pArgsParser->pOptions == NULL)
    {
        FArgumentParserDestroy(&pArgsParser);
        return NULL;
    }

    pArgsParser->pParsedOptions = FAllocateZero(optionsCount, sizeof(FArgumentParsedOption));
    if (pArgsParser->pParsedOptions == NULL)
    {
        FArgumentParserDestroy(&pArgsParser);
        return NULL;
    }

    /* Copy the options */
    pArgsParser->optionsCount = optionsCount;
    FMemoryCopy(pOptions, pArgsParser->pOptions, optionsCount * sizeof(FArgumentOption));

    return pArgsParser;
}

void FArgumentParserDestroy(FArgumentParser** ppArgumentParser)
{
    FArgumentParser* pArgumentParser = *ppArgumentParser;
    if (pArgumentParser == NULL)
    {
        return;
    }

    if (pArgumentParser->pOptions != NULL)
    {
        FDeallocate(pArgumentParser->pOptions);
        pArgumentParser->pOptions = NULL;
    }

    FDeallocate(pArgumentParser);
    *ppArgumentParser = NULL;
}

bool FArgumentParserParse(FArgumentParser* pArgsParser, const int argc, const char* const* argv)
{
    if (pArgsParser == NULL || argc == 0 || argv == NULL)
    {
        return false;
    }

    for (int i = 0; i < argc; i++)
    {
        char* pArg = (char*)argv[i];

        FArgumentParsedOption parsedOption = {0};

        /* Skip empty arguments */
        if (pArg[0] == '\0')
        {
            continue;
        }

        FInt64 argLength = FStringLength(pArg, ENTERPRISE_NAME_MAX_LENGTH);

        /* Make sure that the arguments length is not greater than ENTERPRISE_NAME_MAX_LENGTH */
        if (argLength > ENTERPRISE_NAME_MAX_LENGTH || argLength < 2)
        {
            FLogWarning("An argument was too long!");
            continue;
        }

        /* Skip the first argument */
        if (pArg[0] == '\\' || pArg[0] == '/')
        {
            continue;
        }

        /* Find the "--" before the name */
        if (pArg[0] != '-' || pArg[1] != '-')
        {
            FLogWarning("Failed to parse an argument!");
            continue;
        }
        pArg += 2;


        /* Find the "=" */
        char* pEquals = FStringSeperate(pArg, ENTERPRISE_NAME_MAX_LENGTH, "=", sizeof("="));
        if (pEquals == NULL)
        {
            FLogWarning("Could not get an arguments equal sign!");
            continue;
        }

        /* Get the name */
        char* pName = pArg;
        FInt nameLength = (FInt)(pEquals - pName);

        /* Check the argument name is the same as one of the options */
        FArgumentOption* pOption = NULL;
        for (int j = 0; j < pArgsParser->optionsCount; j++)
        {
            if (FStringCompare(pName, ENTERPRISE_NAME_MAX_LENGTH, pArgsParser->pOptions[j].name, ENTERPRISE_NAME_MAX_LENGTH) == 0)
            {   
                pOption = &pArgsParser->pOptions[j];
                break;
            }
        }

        if (pOption == NULL)
        {
            FLogWarning("An argument was not found in the options!");
            continue;
        }

        /* Set the name in the parsed option */
        FStringCopy(pOption->name, nameLength, parsedOption.name, ENTERPRISE_NAME_MAX_LENGTH);

        /* Check the argument value is not empty */
        if (pArg[0] == '\0')
        {
            FLogWarning("An argument value was empty!");
            continue;
        }

        /* Get the value */
        char* pValue = pEquals + 1;
        FInt valueLength = FStringLength(pValue, ENTERPRISE_NAME_MAX_LENGTH);
        if (valueLength > ENTERPRISE_NAME_MAX_LENGTH)
        {
            FLogWarning("An argument value was too long!");
            continue;
        }

        /* Check the values type */
        FInt valueInteger = 0;
        float valueFloat = 0.0f;
        bool valueBool = false;

        if (FStringIsNumeric(pValue))
        {
            if (FStringConvertToInt(pValue, valueLength, &valueInteger))
            {
                parsedOption.type = E_ARGUMENT_TYPE_INT;
                parsedOption.value.Int = valueInteger;
            }
            else if (FStringConvertToFloat(pValue, valueLength, &valueFloat))
            {
                parsedOption.type = E_ARGUMENT_TYPE_FLOAT;
                parsedOption.value.Float = valueFloat;
            }
        }
        else if (FStringConvertToBool(pValue, valueLength, &valueBool))
        {
            parsedOption.type = E_ARGUMENT_TYPE_BOOL;
            parsedOption.value.Bool = valueBool;
        }
        else
        {
            parsedOption.type = E_ARGUMENT_TYPE_STRING;
            FStringCopy(pValue, valueLength, parsedOption.value.String, ENTERPRISE_NAME_MAX_LENGTH);
        }

        /* Copy the parsed option */
        FMemoryCopy(&parsedOption, &pArgsParser->pParsedOptions[pArgsParser->parsedOptionsCount++], sizeof(FArgumentParsedOption));
    }

    return true;
}

bool FArgumentParserGetOption(FArgumentParser* pArgsParser, const char* pName, FArgumentParsedOption* pParsedOption)
{
    if (pArgsParser == NULL || pName == NULL || pParsedOption == NULL)
    {
        return false;
    }

    for (FInt64 i = 0; i < pArgsParser->parsedOptionsCount; i++)
    {
        if (FStringCompare(pArgsParser->pParsedOptions[i].name, ENTERPRISE_NAME_MAX_LENGTH, pName, ENTERPRISE_NAME_MAX_LENGTH) == 0)
        {
            FMemoryCopy(&pArgsParser->pParsedOptions[i], pParsedOption, sizeof(FArgumentParsedOption));
            return true;
        }
    }

    return false;
}