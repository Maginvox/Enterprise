/* Copyright © 2021 Caden Miller, All Rights Reserved. */

#include "Core/enMacro.h"
#include "Core/enMemory.h"
#include "Core/enMath.h"
#include "Core/enString.h"

bool enCharacterIsSpace(char Character)
{
    return (Character == ' ');
}

bool enCharacterIsDigit(char Character)
{
    return enMathIsBetween(Character, '0', '9');
}

bool enCharacterIsAlpha(char Character)
{
    return enMathIsBetween(Character, 'a', 'z') || enMathIsBetween(Character, 'A', 'Z');
}

bool enCharacterIsAlphaNumeric(char Character)
{
    return enCharacterIsAlpha(Character) || enCharacterIsDigit(Character);
}



/* ====================================================== */
uint32 enStringLength(const char* pString, uint32 maxLength)
{

    if (pString == NULL || maxLength == 0 || maxLength < 0 || maxLength > FSTRING_MAX_LENGTH)
    {
        return 0;
    }

    char* pTemporary = (char*)pString;
    uint32 length = 0;

    /* Count the characters before the NULL terminator */
    while (*pTemporary != '\0' && length < maxLength)
    {
        length++;
        pTemporary++;
    }
    
    return length;
}

/* ====================================================== */
char* enStringSeperate(const char* pSource, uint32 sourceMaxLength, const char* pSeparators, uint32 separatorsLength)
{
    if (pSource == NULL || sourceMaxLength <= 0  || pSeparators == NULL || separatorsLength <= 0)
    {
        return NULL;
    }

    //uint32 sourceLength = enStringLength(pSource, sourceMaxLength);
    //uint32 separatorLength = enStringLength(pSeparators, separatorsLength);

    char* pTemporary = (char*)pSource;
    while (pTemporary != NULL && *pTemporary != '\0')
    {
        for (uint32 i = 0; i < separatorsLength; i++)
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
bool enStringConcatenate(const char* pSource, uint32 sourceMaxLength, char* pDestination, uint32 destinationMaxLength)
{
    if (pSource == NULL || sourceMaxLength <= 0 || pDestination == NULL || destinationMaxLength <= 0)
    {
        return false;
    }

    /* Make sure the destination is large enough */
    uint32 sourceLength = enStringLength(pSource, sourceMaxLength);
    uint32 destinationLength = enStringLength(pDestination, destinationMaxLength);

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
    enStringCopy(pSource, sourceMaxLength, pTemporary, destinationMaxLength);

    return true;
}

/* ====================================================== */
uint32 enStringCompare(const char* pSource, uint32 sourceMaxLength, const char* pCompare, uint32 compareMaxLength)
{
    if (pSource == NULL || sourceMaxLength <= 0 || pCompare == NULL || compareMaxLength <= 0)
    {
        return -1;
    }

    uint32 sourceLength = enStringLength(pSource, sourceMaxLength);
    uint32 compareLength = enStringLength(pCompare, compareMaxLength);

    char* pSourceTemporary = (char*)pSource;
    char* pCompareTemporary = (char*)pCompare;
    uint32 differences = 0;
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
bool enStringCopy(const char* pSource, uint32 sourceMaxLength, char* pDestination, uint32 destinationMaxLength)
{
    if (pSource == NULL || sourceMaxLength <= 0 || pDestination == NULL || destinationMaxLength <= 0)
    {
        return false;
    }

    uint32 sourceLength = enStringLength(pSource, sourceMaxLength);

    if (sourceLength == 0 || sourceLength > destinationMaxLength)
    {
        return false;
    }

    enMemoryCopy(pSource, pDestination, sourceLength);
    pDestination[sourceLength] = '\0';
    return true;
}

/* ====================================================== */
bool enStringReverse(char* pString, uint32 maxLength)
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
    uint32 length = maxLength;
    while (length > 0)
    {
        length--;
        pTemporary[length] = pString[length];
    }
    return true;
}

/* ====================================================== */
void enStringRemoveSpaces(char* pString)
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
bool enStringIsAlpha(const char* pString)
{
    if (pString == NULL)
    {
        return false;
    }

    uint32 maxLength = enStringLength(pString, FSTRING_MAX_LENGTH);

    if (!enCharacterIsAlpha(*pString))
    {
        return false;
    }

    char* pStringTemporary = (char*)pString;
    while (*pStringTemporary != '\0' && (pStringTemporary - pString) < maxLength)
    {
        if (!enCharacterIsAlpha(*pStringTemporary))
        {
            return false;
        }

        pStringTemporary++;
    }

    return true;
}

/* ====================================================== */
bool enStringIsNumeric(const char* pString)
{
    if (pString == NULL)
    {
        return false;
    }

    uint32 maxLength = enStringLength(pString, FSTRING_MAX_LENGTH);

    char* pStringTemporary = (char*)pString;
    while (*pStringTemporary != '\0' && (pStringTemporary - pString) < maxLength)
    {
        if (!enCharacterIsDigit(*pStringTemporary) && (*pStringTemporary) != '.')
        {
            return false;
        }
        pStringTemporary++;
    }

    return true;
}

/* ====================================================== */
bool enStringIsAlphaNumeric(const char* pString)
{
    if (pString == NULL)
    {
        return false;
    }

    return enStringIsAlpha(pString) || enStringIsNumeric(pString);
}

/* ====================================================== */
bool enStringFormat(char* pDestination, uint32 destinationMaxLength, const char* pFormat, const enStringFormatArgument* pFormatArguments, uint32 formatArgumentsCount)
{
    if (pDestination == NULL || destinationMaxLength <= 0 || pFormat == NULL || formatArgumentsCount <= 0 || pFormatArguments == NULL)
    {
        return false;
    }

    static const char pFormatTypes[] = { 'i', 'f', 'd', 's' };

    char* pFormatTemporary = (char*)pFormat;
    uint32 usedLength = 0;
    uint32 formatLength = enStringLength(pFormat, FSTRING_MAX_LENGTH);

    /* We have to find each of the arguments in the format string */
    /* We must ensure that the format and the arguments are in order and are the same type */
    for (uint32 i = 0; i < formatArgumentsCount; i++)
    {
        const enStringFormatArgument* pFormatArgument = &pFormatArguments[i];        

        /* Find the argument in the format string */
        if (!enMathIsBetween(pFormatArgument->type, 0, enStringFormatType_Max))
        {
            return false; /* Invalid type */
        }

        char* pFormatType = enStringSeperate(pFormatTemporary, formatLength, "%", sizeof("%"));
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

        enStringCopy(pFormatTemporary, (uint32)(pFormatType - pFormatTemporary), pDestination + usedLength, destinationMaxLength);
  
        usedLength += (uint32)(pFormatType - pFormatTemporary);
        pFormatTemporary += (2 + pFormatType - pFormatTemporary);

        /* Concatenate the argument to the destination */
        char pBuffer[32] = {0};
        uint32 stringLength = 0;
        switch (pFormatArgument->type)
        {
            case enStringFormatType_Int:
                enStringConvertFromInt(pFormatArgument->value.Integer, 10, pBuffer, sizeof(pBuffer));
                uint32 integerLength = enStringLength(pBuffer, sizeof(pBuffer));
                enStringConcatenate(pBuffer, sizeof(pBuffer), pDestination, destinationMaxLength);
                usedLength += integerLength;
                break;
            case enStringFormatType_Float:
                enStringConvertFromFloat(pFormatArgument->value.Float, pBuffer, sizeof(pBuffer));
                uint32 floatLength = enStringLength(pBuffer, sizeof(pBuffer));
                enStringConcatenate(pBuffer, sizeof(pBuffer), pDestination, destinationMaxLength);
                usedLength += floatLength;
                break;
            case enStringFormatType_Double:
                enStringConvertFromDouble(pFormatArgument->value.Double, pBuffer, sizeof(pBuffer));
                uint32 doubleLength = enStringLength(pBuffer, sizeof(pBuffer));
                enStringConcatenate(pBuffer, sizeof(pBuffer), pDestination, destinationMaxLength);
                usedLength += doubleLength;
                break;
            case enStringFormatType_String:
                stringLength = enStringLength(pFormatArgument->value.String, FSTRING_MAX_LENGTH);
                enStringConcatenate(pFormatArgument->value.String, stringLength, pDestination, destinationMaxLength);
                usedLength += stringLength;
                break;
        }
    }

    /* Copy everything after the arguments */
    uint32 remainingInFormat = enStringLength(pFormatTemporary, FSTRING_MAX_LENGTH - usedLength);
    if (remainingInFormat + usedLength >= destinationMaxLength)
    {
        return false; /* Destination is too small */
    }

    enStringConcatenate(pFormatTemporary, remainingInFormat, pDestination, destinationMaxLength);
    usedLength += remainingInFormat;

    /* NULL terminate the string */
    pDestination[usedLength] = '\0';
    return true;
}


/* ====================================================== */
bool enStringConvertFromInt(int64 value, int8 base, char* pDestination, uint32 destinationMaxLength)
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
    int64 temporaryValue = 0;

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
bool enStringConvertFromFloat(float value, char* pDestination, uint32 destinationMaxLength)
{
    if (pDestination == NULL || destinationMaxLength <= 0)
    {
        return false;
    }

    uint32 valueInt = (uint32)value;
    float valueFraction = (value - (float)valueInt);
    
    if (!enStringConvertFromInt(valueInt, 10, pDestination, destinationMaxLength))
    {
        return false;
    }

    /* Get the string length */
    uint32 stringLength = enStringLength(pDestination, destinationMaxLength);
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
        valueFraction = enFloatMathPow(10, valueFraction);

        return enStringConvertFromInt((uint32)valueFraction, 10, pDestination + stringLength + 1, destinationMaxLength);
    }

    return true;
}

/* ====================================================== */
bool enStringConvertFromDouble(double value, char* pDestination, uint32 destinationMaxLength)
{
    if (pDestination == NULL || destinationMaxLength <= 0)
    {
        return false;
    }

    uint32 valueInt = (uint32)value;
    double valueFraction = (value - (double)valueInt);

    if (!enStringConvertFromInt(valueInt, 10, pDestination, destinationMaxLength))
    {
        return false;
    }

    /* Get the string length */
    uint32 stringLength = enStringLength(pDestination, destinationMaxLength);
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
        valueFraction = enMathDoublePow(10, valueFraction);
        return enStringConvertFromInt((uint32)valueFraction, 10, pDestination + stringLength + 1, destinationMaxLength);
    }

    return true;
}

/* ====================================================== */
bool enStringConvertFromBool(bool value, char* pDestination, uint32 destinationMaxLength)
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

    return enStringCopy(pTrueFalse[value ? 0 : 1], sizeof(pTrueFalse[0]), pDestination, destinationMaxLength);    
}

/* ====================================================== */
bool enStringConvertFromTime(const enTime* pTime, char* pDestination, uint32 destinationMaxLength)
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

    const enStringFormatArgument pFormatArguments[] =
    {
        {enStringFormatType_String, .value.String = (char*)dayName[pTime->weekDay]},
        {enStringFormatType_String, .value.String = (char*)monthName[pTime->month - 1]},
        {enStringFormatType_Int, .value.Integer = pTime->monthDay},
        {enStringFormatType_Int, .value.Integer = pTime->hour},
        {enStringFormatType_Int, .value.Integer = pTime->minute},
        {enStringFormatType_Int, .value.Integer = pTime->second},
        {enStringFormatType_Int, .value.Integer = pTime->year}
    };

    char timeFormat[] = "%s %s %i %i:%i:%i %i";
    enStringFormat(pDestination, destinationMaxLength, timeFormat, pFormatArguments, COUNT_OF(pFormatArguments));

    return true;
}

/* ====================================================== */
bool enStringConvertToInt(const char* pSource, uint32 sourceMaxLength, int64* pValue)
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
    uint32 index = 0;
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
    uint32 signature = 1;
    if (pSource[index] == '-' || pSource[index] == '+')
    {
        signature = 1 - 2 * (pSource[index++] == '-');
    }

    uint32 base = 0;
    while (index <= sourceMaxLength && pSource[index] >= '0' && pSource[index] <= '9')
    {
        if(pSource[index] == '.') /* This is not an integer */
        {
            return false;
        }

        /* Handle overflowing */
        if (base > ENTERPRISE_INT64_MAX / 10 || (base == ENTERPRISE_INT64_MAX / 10 && pSource[index] - '0' > 7))
        {
            if (signature == 1)
            {
                *pValue = ENTERPRISE_INT64_MAX;
            }
            else
            {
                *pValue = ENTERPRISE_INT64_MIN;
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
bool enStringConvertToFloat(const char* pSource, uint32 sourceMaxLength, float* pValue)
{
    if (pSource == NULL || sourceMaxLength <= 0 || pValue == NULL)
    {
        return false;
    }


    float value = 0;
    uint32 afterDot = 0;
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
bool enStringConvertToDouble(const char* pSource, uint32 sourceMaxLength, double* pValue)
{
    if (pSource == NULL || sourceMaxLength <= 0 || pValue == NULL)
    {
        return false;
    }


    double value = 0;
    uint32 afterDot = 0;
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
bool enStringConvertToBool(const char* pSource, uint32 sourceMaxLength, bool* pValue)
{
    if (pSource == NULL || sourceMaxLength <= 0 || pValue == NULL)
    {
        return false;
    }

    if (enStringCompare(pSource, sourceMaxLength, "True", sizeof("True")) == 0)
    {
        *pValue = true;
    }
    else if (enStringCompare(pSource, sourceMaxLength, "False", sizeof("False")) == 0)
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
uint3264 FWideStringLength(const wchar* pString, unsigned long long MaxLength)
{

}

wchar* FWideStringTokenize(const wchar* pSource, unsigned long long* pSourceMaxLength, const wchar* pDeliminator, wchar** ppToken)
{
    
}

bool FWideStringConcatenate(const wchar* pSource, unsigned long long sourceMaxLength, wchar* pDestination, unsigned long long destinationMaxLength)
{
    
}

bool FWideStringCompare(const wchar* pSource, unsigned long long sourceMaxLength, const wchar* pCompare, unsigned long long compareMaxLength)
{
    
}

bool FWideStringCopy(const wchar* pSource, unsigned long long sourceMaxLength, wchar* pDestination, unsigned long long destinationMaxLength)
{
    
}

bool FWideStringConvertFromBool(const bool value, wchar* pDestination, uint32 destinationMaxLength)
{

}

bool FWideStringConvertFromLong(const int32 value, wchar* pDestination, uint32 destinationMaxLength)
{

}

bool FWideStringConvertFromLongLong(const uint32 value, wchar* pDestination, uint32 destinationMaxLength)
{

}

bool FWideStringConvertFromUnsignedLong(const uint3232 value, wchar* pDestination, uint32 destinationMaxLength)
{

}

bool FWideStringConvertFromUnsignedLongLong(const uint32 value, wchar* pDestination, uint32 destinationMaxLength)
{

}

bool FWideStringConvertToBool(const wchar* pSource, uint32 sourceMaxLength, bool *pDestination)
{

}

bool FWideStringConvertToLong(const wchar* pSource, uint32 sourceMaxLength, int32* pDestination)
{

}

bool FWideStringConvertToLongLong(const wchar* pSource, uint32 sourceMaxLength, uint32* pDestination)
{

}

bool FWideStringConvertToUnsignedLong(const wchar* pSource, uint32 sourceMaxLength, uint3232* pDestination)
{

}

bool FWideStringConvertToUnsignedLongLong(const wchar* pSource, uint32 sourceMaxLength, uint3264* pDestination)
{

}
*/