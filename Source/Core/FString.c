#include "Core/FMacro.h"
#include "Core/FMemory.h"
#include "Core/FMath.h"
#include "Core/FString.h"

bool FCharacterIsSpace(char Character)
{
    return (Character == ' ');
}

bool FCharacterIsDigit(char Character)
{
    return FMathIsBetween(Character, '0', '9');
}

bool FCharacterIsAlpha(char Character)
{
    return FMathIsBetween(Character, 'a', 'z') || FMathIsBetween(Character, 'A', 'Z');
}

bool FCharacterIsAlphaNumeric(char Character)
{
    return FCharacterIsAlpha(Character) || FCharacterIsDigit(Character);
}



/* ====================================================== */
FInt FStringLength(const char* pString, FInt maxLength)
{

    if (pString == NULL || maxLength == 0 || maxLength < 0 || maxLength > FSTRING_MAX_LENGTH)
    {
        return 0;
    }

    char* pTemporary = (char*)pString;
    FInt length = 0;

    /* Count the characters before the NULL terminator */
    while (*pTemporary != '\0' && length < maxLength)
    {
        length++;
        pTemporary++;
    }
    
    return length;
}

/* ====================================================== */
char* FStringSeperate(const char* pSource, FInt sourceMaxLength, const char* pSeparators, FInt separatorsLength)
{
    if (pSource == NULL || sourceMaxLength <= 0  || pSeparators == NULL || separatorsLength <= 0)
    {
        return NULL;
    }

    //FInt sourceLength = FStringLength(pSource, sourceMaxLength);
    //FInt separatorLength = FStringLength(pSeparators, separatorsLength);

    char* pTemporary = (char*)pSource;
    while (pTemporary != NULL && *pTemporary != '\0')
    {
        for (FInt i = 0; i < separatorsLength; i++)
        {
            if (*pTemporary == pSeparators[i])
            {
                return pTemporary;
            }
        }
        
        pTemporary++;
    }

    return NULL;
}

/* ====================================================== */
bool FStringConcatenate(const char* pSource, FInt sourceMaxLength, char* pDestination, FInt destinationMaxLength)
{
    if (pSource == NULL || sourceMaxLength <= 0 || pDestination == NULL || destinationMaxLength <= 0)
    {
        return false;
    }

    /* Make sure the destination is large enough */
    FInt sourceLength = FStringLength(pSource, sourceMaxLength);
    FInt destinationLength = FStringLength(pDestination, destinationMaxLength);

    if (sourceLength + destinationLength >= destinationMaxLength)
    {
        return false;
    }

    /* Move the temporary pointer to the end of the string */
    char* pTemporary = (char*)pDestination;
    while (*pTemporary != '\0')
    {
        pTemporary++;
    }

    /* Copy the source to the destination */
    FStringCopy(pSource, sourceMaxLength, pTemporary, destinationMaxLength);

    return true;
}

/* ====================================================== */
FInt FStringCompare(const char* pSource, FInt sourceMaxLength, const char* pCompare, FInt compareMaxLength)
{
    if (pSource == NULL || sourceMaxLength <= 0 || pCompare == NULL || compareMaxLength <= 0)
    {
        return -1;
    }

    FInt sourceLength = FStringLength(pSource, sourceMaxLength);
    FInt compareLength = FStringLength(pCompare, compareMaxLength);

    char* pSourceTemporary = (char*)pSource;
    char* pCompareTemporary = (char*)pCompare;
    FInt differences = 0;
    while (sourceLength > 0 && compareLength > 0 && *pSourceTemporary != '\0' && *pCompareTemporary != '\0')
    {
        if (*pSourceTemporary != *pCompareTemporary)
        {
            differences++;
        }
        pSourceTemporary++;
        pCompareTemporary++;
        sourceLength--;
        compareLength--;
    }

    return differences;
}

/* ====================================================== */
bool FStringCopy(const char* pSource, FInt sourceMaxLength, char* pDestination, FInt destinationMaxLength)
{
    if (pSource == NULL || sourceMaxLength <= 0 || pDestination == NULL || destinationMaxLength <= 0)
    {
        return false;
    }

    FInt sourceLength = FStringLength(pSource, sourceMaxLength);

    if (sourceLength == 0 || sourceLength > destinationMaxLength)
    {
        return false;
    }

    FMemoryCopy(pSource, pDestination, sourceLength);
    pDestination[sourceLength] = '\0';
    return true;
}

/* ====================================================== */
bool FStringReverse(char* pString, FInt maxLength)
{
    if (pString == NULL || maxLength <= 0)
    {
        return false;
    }

    /* Check for NULL termination. */
    if (pString[maxLength - 1] != '\0')
    {
        return false;
    }

    char* pTemporary = pString;
    FInt length = maxLength;
    while (length > 0)
    {
        length--;
        pTemporary[length] = pString[length];
    }
    return true;
}

/* ====================================================== */
void FStringRemoveSpaces(char* pString)
{
    if (pString == NULL)
    {
        return;
    }

    char* pTemporary = pString;
    while ((*pString++ = *pTemporary++))
    {
        while(*pTemporary == ' ')
        {
            pTemporary++;   
        }
    }

    /* NULL terminate the string */
    *pString = '\0';
}

/* ====================================================== */
bool FStringIsAlpha(const char* pString)
{
    if (pString == NULL)
    {
        return false;
    }

    FInt maxLength = FStringLength(pString, FSTRING_MAX_LENGTH);

    if (!FCharacterIsAlpha(*pString))
    {
        return false;
    }

    char* pStringTemporary = (char*)pString;
    while (*pStringTemporary != '\0' && (pStringTemporary - pString) < maxLength)
    {
        if (!FCharacterIsAlpha(*pStringTemporary))
        {
            return false;
        }

        pStringTemporary++;
    }

    return true;
}

/* ====================================================== */
bool FStringIsNumeric(const char* pString)
{
    if (pString == NULL)
    {
        return false;
    }

    FInt maxLength = FStringLength(pString, FSTRING_MAX_LENGTH);

    char* pStringTemporary = (char*)pString;
    while (*pStringTemporary != '\0' && (pStringTemporary - pString) < maxLength)
    {
        if (!FCharacterIsDigit(*pStringTemporary) && (*pStringTemporary) != '.')
        {
            return false;
        }
        pStringTemporary++;
    }

    return true;
}

/* ====================================================== */
bool FStringIsAlphaNumeric(const char* pString)
{
    if (pString == NULL)
    {
        return false;
    }

    return FStringIsAlpha(pString) || FStringIsNumeric(pString);
}

/* ====================================================== */
bool FStringFormat(char* pDestination, FInt destinationMaxLength, const char* pFormat, const FStringFormatArgument* pFormatArguments, FInt formatArgumentsCount)
{
    if (pDestination == NULL || destinationMaxLength <= 0 || pFormat == NULL || formatArgumentsCount <= 0 || pFormatArguments == NULL)
    {
        return false;
    }

    static const char pFormatTypes[] = { 'i', 'f', 'd', 's' };

    char* pFormatTemporary = (char*)pFormat;
    FInt usedLength = 0;
    FInt formatLength = FStringLength(pFormat, FSTRING_MAX_LENGTH);

    /* We have to find each of the arguments in the format string */
    /* We must ensure that the format and the arguments are in order and are the same type */
    for (FInt i = 0; i < formatArgumentsCount; i++)
    {
        const FStringFormatArgument* pFormatArgument = &pFormatArguments[i];        

        /* Find the argument in the format string */
        if (!FMathIsBetween(pFormatArgument->type, 0, E_STRING_FORMAT_TYPE_MAX))
        {
            return false; /* Invalid type */
        }

        char* pFormatType = FStringSeperate(pFormatTemporary, formatLength, "%", sizeof("%"));
        if (pFormatType == NULL || pFormatType[1] == '\0' || pFormatType[1] == '%')
        {
            return false; /* Invalid format */
        }
        
        char pFormatTypeChar = pFormatType[1];

        /* Ensure the argument types are the same */
        if (pFormatTypeChar != pFormatTypes[pFormatArguments[i].type])
        {
            return false; /* Invalid format */
        }

        /* Copy everything before the "%" into the destination */
        if ((pFormatTemporary - pFormatType) + usedLength >= destinationMaxLength)
        {
            return false; /* Destination is too small */
        }

        FStringCopy(pFormatTemporary, (FInt)(pFormatType - pFormatTemporary), pDestination + usedLength, destinationMaxLength);
  
        usedLength += (FInt)(pFormatType - pFormatTemporary);
        pFormatTemporary += (2 + pFormatType - pFormatTemporary);

        /* Concatenate the argument to the destination */
        char pBuffer[32] = {0};
        FInt stringLength = 0;
        switch (pFormatArgument->type)
        {
            case E_STRING_FORMAT_TYPE_INT:
                FStringConvertFromInt(pFormatArgument->value.Integer, 10, pBuffer, sizeof(pBuffer));
                FInt integerLength = FStringLength(pBuffer, sizeof(pBuffer));
                FStringConcatenate(pBuffer, sizeof(pBuffer), pDestination, destinationMaxLength);
                usedLength += integerLength;
                break;
            case E_STRING_FORMAT_TYPE_FLOAT:
                FStringConvertFromFloat(pFormatArgument->value.Float, pBuffer, sizeof(pBuffer));
                FInt floatLength = FStringLength(pBuffer, sizeof(pBuffer));
                FStringConcatenate(pBuffer, sizeof(pBuffer), pDestination, destinationMaxLength);
                usedLength += floatLength;
                break;
            case E_STRING_FORMAT_TYPE_DOUBLE:
                FStringConvertFromDouble(pFormatArgument->value.Double, pBuffer, sizeof(pBuffer));
                FInt doubleLength = FStringLength(pBuffer, sizeof(pBuffer));
                FStringConcatenate(pBuffer, sizeof(pBuffer), pDestination, destinationMaxLength);
                usedLength += doubleLength;
                break;
            case E_STRING_FORMAT_TYPE_STRING:
                stringLength = FStringLength(pFormatArgument->value.String, FSTRING_MAX_LENGTH);
                FStringConcatenate(pFormatArgument->value.String, stringLength, pDestination, destinationMaxLength);
                usedLength += stringLength;
                break;
        }
    }

    /* Copy everything after the arguments */
    FInt remainingInFormat = FStringLength(pFormatTemporary, FSTRING_MAX_LENGTH - usedLength);
    if (remainingInFormat + usedLength >= destinationMaxLength)
    {
        return false; /* Destination is too small */
    }

    FStringConcatenate(pFormatTemporary, remainingInFormat, pDestination, destinationMaxLength);
    usedLength += remainingInFormat;

    /* NULL terminate the string */
    pDestination[usedLength] = '\0';
    return true;
}


/* ====================================================== */
bool FStringConvertFromInt(FInt value, FInt8 base, char* pDestination, FInt destinationMaxLength)
{
    if (base < 2 || base > 36 || pDestination == NULL || destinationMaxLength <= 0)
    {
        return false;
    }

    /* Check for NULL termination. */
    if (pDestination[destinationMaxLength - 1] != '\0')
    {
        *pDestination = '\0';
        return false;
    }

    char* pTemporary = pDestination;
    FInt temporaryValue = 0;

    do {
        temporaryValue = value;
        value /= base;
        *pTemporary++ = "zyxwvutsrqponmlkjihgfedcba9876543210123456789abcdefghijklmnopqrstuvwxyz" [35 + (temporaryValue - value * base)];
    } while (value);

    /* Negative sign */
    if (temporaryValue < 0)
    {
        *pTemporary++ = '-';
    }
    *pTemporary-- = '\0';

    /* Reverse the string */
    char reversedChar;
    char* pReversed = pDestination;
    while (pReversed < pTemporary)
    {
        reversedChar = *pTemporary;
        *pTemporary-- = *pReversed;
        *pReversed++ = reversedChar;
    }

    return true;
}

/* ====================================================== */
bool FStringConvertFromFloat(float value, char* pDestination, FInt destinationMaxLength)
{
    if (pDestination == NULL || destinationMaxLength <= 0)
    {
        return false;
    }

    FInt valueInt = (FInt)value;
    float valueFraction = (value - (float)valueInt);
    
    if (!FStringConvertFromInt(valueInt, 10, pDestination, destinationMaxLength))
    {
        return false;
    }

    /* Get the string length */
    FInt stringLength = FStringLength(pDestination, destinationMaxLength);
    if (stringLength >= destinationMaxLength)
    {
        return false;
    }

    /* Add the decimal point */
    if (valueFraction != 0)
    {
        if (stringLength + 1 <= destinationMaxLength)
        {
            pDestination[stringLength] = '.';
        }

        /* Add the fractional part */
        valueFraction = FFloatMathPow(10, valueFraction);

        return FStringConvertFromInt((FInt)valueFraction, 10, pDestination + stringLength + 1, destinationMaxLength);
    }

    return true;
}

/* ====================================================== */
bool FStringConvertFromDouble(double value, char* pDestination, FInt destinationMaxLength)
{
    if (pDestination == NULL || destinationMaxLength <= 0)
    {
        return false;
    }

    FInt valueInt = (FInt)value;
    double valueFraction = (value - (double)valueInt);

    if (!FStringConvertFromInt(valueInt, 10, pDestination, destinationMaxLength))
    {
        return false;
    }

    /* Get the string length */
    FInt stringLength = FStringLength(pDestination, destinationMaxLength);
    if (stringLength >= destinationMaxLength)
    {
        return false;
    }

    /* Add the decimal point */
    if (valueFraction != 0)
    {
        if (stringLength + 1 <= destinationMaxLength)
        {
            pDestination[stringLength] = '.';
        }

        /* Add the fractional part */
        valueFraction = FMathDoublePow(10, valueFraction);
        return FStringConvertFromInt((FInt)valueFraction, 10, pDestination + stringLength + 1, destinationMaxLength);
    }

    return true;
}

/* ====================================================== */
bool FStringConvertFromBool(bool value, char* pDestination, FInt destinationMaxLength)
{
    if (pDestination == NULL || destinationMaxLength <= 0)
    {
        return false;
    }

    const char pTrueFalse[2][6] =
    {
        "True",
        "False"
    };

    return FStringCopy(pTrueFalse[value ? 0 : 1], sizeof(pTrueFalse[0]), pDestination, destinationMaxLength);    
}

/* ====================================================== */
bool FStringConvertFromTime(const FTime* pTime, char* pDestination, FInt destinationMaxLength)
{
    if (pTime == NULL || pDestination == NULL || destinationMaxLength < 32)
    {
        return false;
    }

    static const char dayName[7][4] = { 
        "Sun", "Mon", "Tue", "Wed", "Thu", "Fri", "Sat"
    };

    static const char monthName[12][4] = {
        "Jan", "Feb", "Mar", "Apr", "May", "Jun", 
        "Jul", "Aug", "Sep", "Oct", "Nov", "Dec" 
    };

    const FStringFormatArgument pFormatArguments[] =
    {
        {E_STRING_FORMAT_TYPE_STRING, .value.String = (char*)dayName[pTime->weekDay]},
        {E_STRING_FORMAT_TYPE_STRING, .value.String = (char*)monthName[pTime->month - 1]},
        {E_STRING_FORMAT_TYPE_INT, .value.Integer = pTime->monthDay},
        {E_STRING_FORMAT_TYPE_INT, .value.Integer = pTime->hour},
        {E_STRING_FORMAT_TYPE_INT, .value.Integer = pTime->minute},
        {E_STRING_FORMAT_TYPE_INT, .value.Integer = pTime->second},
        {E_STRING_FORMAT_TYPE_INT, .value.Integer = pTime->year}
    };

    char timeFormat[] = "%s %s %i %i:%i: %i %i";
    FStringFormat(pDestination, destinationMaxLength, timeFormat, pFormatArguments, FCOUNT_OF(pFormatArguments));

    return true;
}

/* ====================================================== */
bool FStringConvertToInt(const char* pSource, FInt sourceMaxLength, FInt* pValue)
{
    if (pSource == NULL || sourceMaxLength <= 0 || pValue == NULL)
    {
        return false;
    }

    if (pSource[0] == '\0')
    {
        return false;
    }

    /* Ignore possible whitespaces int the beginning */
    FInt index = 0;
    while (index < sourceMaxLength && pSource[index] == ' ')
    {
        index++;
    }

    /* Check if the string is empty */
    if (index >= sourceMaxLength)
    {
        return false;
    }

    /* Get the signature of the number */
    FInt signature = 1;
    if (pSource[index] == '-' || pSource[index] == '+')
    {
        signature = 1 - 2 * (pSource[index++] == '-');
    }

    FInt base = 0;
    while (index <= sourceMaxLength && pSource[index] >= '0' && pSource[index] <= '9')
    {
        if(pSource[index] == '.') /* This is not an integer */
        {
            return false;
        }

        /* Handle overflowing */
        if (base > FINT32_MAX / 10 || (base == FINT32_MAX / 10 && pSource[index] - '0' > 7))
        {
            if (signature == 1)
            {
                *pValue = FINT32_MAX;
            }
            else
            {
                *pValue = FINT32_MIN;
            }
        }
        base = base * 10 + (pSource[index++] - '0');
    }
    *(pValue) = base * signature;

    if (base * signature == 0 && pSource[index + 1] != '\0' && pSource[index + 1] != '.')
    {
        return false; /* This is not an integer */
    }

    return true;
}

/* ====================================================== */
bool FStringConvertToFloat(const char* pSource, FInt sourceMaxLength, float* pValue)
{
    if (pSource == NULL || sourceMaxLength <= 0 || pValue == NULL)
    {
        return false;
    }


    float value = 0;
    FInt afterDot = 0;
    float scale = 1;
    bool negative = 0;     
    char* pSourceTemporary = (char*)pSource;

    /* Check for negative symbol */
    if (*pSourceTemporary == '-') {
        pSourceTemporary++;
        negative = true;
    }


    while (*pSourceTemporary != '\0' && pSourceTemporary - pSource < sourceMaxLength) {
        if (afterDot) {
            scale = scale / 10;
            value = value + (*pSourceTemporary - '0') * scale;
        } 
        else 
        {
            if (*pSourceTemporary == '.')
            { 
                afterDot++;
            }
            else
            {
                value = value * 10.0f + (*pSourceTemporary - '0');
            }
        }
        pSourceTemporary++;
    }

    *pValue = negative ? -value : value;
    return true;
}

/* ====================================================== */
bool FStringConvertToDouble(const char* pSource, FInt sourceMaxLength, double* pValue)
{
    if (pSource == NULL || sourceMaxLength <= 0 || pValue == NULL)
    {
        return false;
    }


    double value = 0;
    FInt afterDot = 0;
    double scale = 1;
    bool negative = 0;     
    char* pSourceTemporary = (char*)pSource;

    /* Check for negative symbol */
    if (*pSourceTemporary == '-') {
        pSourceTemporary++;
        negative = true;
    }

    while (*pSourceTemporary != '\0' && pSourceTemporary - pSource < sourceMaxLength) 
    {
        /* If we are after the dot we must deal with fractions properly */
        if (afterDot) {
            scale = scale / 10.0;
            value = value + (*pSourceTemporary - '0') * scale;
        } 
        else 
        {
            if (*pSourceTemporary == '.')
            { 
                afterDot++;
            }
            else
            {
                value = value * 10.0 + (*pSourceTemporary - '0');
            }
        }
        
        pSourceTemporary++;
    }

    *pValue = negative ? -value : value;
    return true;
}

/* ====================================================== */
bool FStringConvertToBool(const char* pSource, FInt sourceMaxLength, bool* pValue)
{
    if (pSource == NULL || sourceMaxLength <= 0 || pValue == NULL)
    {
        return false;
    }

    if (FStringCompare(pSource, sourceMaxLength, "True", sizeof("True")) == 0)
    {
        *pValue = true;
    }
    else if (FStringCompare(pSource, sourceMaxLength, "False", sizeof("False")) == 0)
    {
        *pValue = false;
    }
    else
    {
        return false;
    }
    return true;
}


/* ============================================================================================================ */
/* ============================================================================================================ */
/* ============================================================================================================ */

/*
FUInt64 FWideStringLength(const FWChar* pString, unsigned long long MaxLength)
{

}

FWChar* FWideStringTokenize(const FWChar* pSource, unsigned long long* pSourceMaxLength, const FWChar* pDeliminator, FWChar** ppToken)
{
    
}

bool FWideStringConcatenate(const FWChar* pSource, unsigned long long sourceMaxLength, FWChar* pDestination, unsigned long long destinationMaxLength)
{
    
}

bool FWideStringCompare(const FWChar* pSource, unsigned long long sourceMaxLength, const FWChar* pCompare, unsigned long long compareMaxLength)
{
    
}

bool FWideStringCopy(const FWChar* pSource, unsigned long long sourceMaxLength, FWChar* pDestination, unsigned long long destinationMaxLength)
{
    
}

bool FWideStringConvertFromBool(const bool value, FWChar* pDestination, FInt destinationMaxLength)
{

}

bool FWideStringConvertFromLong(const FInt32 value, FWChar* pDestination, FInt destinationMaxLength)
{

}

bool FWideStringConvertFromLongLong(const FInt value, FWChar* pDestination, FInt destinationMaxLength)
{

}

bool FWideStringConvertFromUnsignedLong(const FUInt32 value, FWChar* pDestination, FInt destinationMaxLength)
{

}

bool FWideStringConvertFromUnsignedLongLong(const FInt value, FWChar* pDestination, FInt destinationMaxLength)
{

}

bool FWideStringConvertToBool(const FWChar* pSource, FInt sourceMaxLength, bool *pDestination)
{

}

bool FWideStringConvertToLong(const FWChar* pSource, FInt sourceMaxLength, FInt32* pDestination)
{

}

bool FWideStringConvertToLongLong(const FWChar* pSource, FInt sourceMaxLength, FInt* pDestination)
{

}

bool FWideStringConvertToUnsignedLong(const FWChar* pSource, FInt sourceMaxLength, FUInt32* pDestination)
{

}

bool FWideStringConvertToUnsignedLongLong(const FWChar* pSource, FInt sourceMaxLength, FUInt64* pDestination)
{

}
*/