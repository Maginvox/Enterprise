/* Copyright © 2021 Caden Miller, All Rights Reserved. */

#ifndef __FSTRING_H__
#define __FSTRING_H__

#include "Core/FTypes.h"
#include "Core/FTime.h"

#define FSTRING_MAX_LENGTH 1024

typedef enum EStringFormatType
{
    E_STRING_FORMAT_TYPE_INT = 0,
    E_STRING_FORMAT_TYPE_FLOAT,
    E_STRING_FORMAT_TYPE_DOUBLE,
    E_STRING_FORMAT_TYPE_STRING,
    E_STRING_FORMAT_TYPE_MAX = E_STRING_FORMAT_TYPE_STRING
} EStringFormatType;

typedef struct FStringFormatArgument
{
    EStringFormatType type;
    union
    {
        int64 Integer;
        double Double;
        float Float;
        char* String;
    } value;
} FStringFormatArgument;

bool FCharacterIsSpace(char Character);
bool FCharacterIsDigit(char Character);
bool FCharacterIsAlpha(char Character);
bool FCharacterIsAlphaNumeric(char Character);

uint32 FStringLength(const char* pString, uint32 maxLength);
char* FStringSeperate(const char* pSource, uint32 sourceMaxLength, const char* pSeparators, uint32 separatorsLength);
bool FStringConcatenate(const char* pSource, uint32 sourceMaxLength, char* pDestination, uint32 destinationMaxLength);
uint32 FStringCompare(const char* pSource, uint32 sourceMaxLength, const char* pCompare, uint32 compareMaxLength);
bool FStringCopy(const char* pSource, uint32 sourceMaxLength, char* pDestination, uint32 destinationMaxLength);
bool FStringReverse(char* pString, uint32 maxLength);
void FStringRemoveSpaces(char* pString);
bool FStringIsAlpha(const char* pString);
bool FStringIsNumeric(const char* pString);
bool FStringIsAlphaNumeric(const char* pString);
bool FStringFormat(char* pDestination, uint32 destinationMaxLength, const char* pFormat, const FStringFormatArgument* pFormatArguments, uint32 formatArgumentsCount); /* Possible escape formats, %i for int, %f for float, %d for double, and %s for string */

bool FStringConvertFromInt(int64 value, int8 base, char* pDestination, uint32 destinationMaxLength);
bool FStringConvertFromFloat(float value, char* pDestination, uint32 destinationMaxLength);
bool FStringConvertFromDouble(double value, char* pDestination, uint32 destinationMaxLength);
bool FStringConvertFromBool(bool value, char* pDestination, uint32 destinationMaxLength);
bool FStringConvertFromTime(const FTime* pValue, char* pDestination, uint32 destinationMaxLength); /* Destination must be greater than or equal to 32 characters */

bool FStringConvertToInt(const char* pSource, uint32 sourceMaxLength, int64* pValue);
bool FStringConvertToFloat(const char* pSource, uint32 sourceMaxLength, float* pValue);
bool FStringConvertToDouble(const char* pSource, uint32 sourceMaxLength, double* pValue);
bool FStringConvertToBool(const char* pSource, uint32 sourceMaxLength, bool* pValue);



/*
FInt FWideStringLength(const wchar* pString, FInt MaxLength);
wchar* FWideStringTokenize(const wchar* pSource, FInt* pSourceMaxLength, const wchar* pDeliminator, wchar** ppToken);
bool FWideStringConcatenate(const wchar* pSource, FInt sourceMaxLength, wchar* pDestination, FInt destinationMaxLength);
bool FWideStringCompare(const wchar* pSource, FInt sourceMaxLength, const wchar* pCompare, FInt compareMaxLength);
bool FWideStringCopy(const wchar* pSource, FInt sourceMaxLength, wchar* pDestination, FInt destinationMaxLength);

bool FWideStringConvertFromBool(const bool value, wchar* pDestination, FInt destinationMaxLength);
bool FWideStringConvertFromLong(const int32 value, wchar* pDestination, FInt destinationMaxLength);
bool FWideStringConvertFromLongLong(const FInt value, wchar* pDestination, FInt destinationMaxLength);
bool FWideStringConvertFromUnsignedLong(const uint3232 value, wchar* pDestination, FInt destinationMaxLength);
bool FWideStringConvertFromUnsignedLongLong(const FInt value, wchar* pDestination, FInt destinationMaxLength);
bool FWideStringConvertToBool(const wchar* pSource, FInt sourceMaxLength, bool *pDestination);
bool FWideStringConvertToLong(const wchar* pSource, FInt sourceMaxLength, int32* pDestination);
bool FWideStringConvertToLongLong(const wchar* pSource, FInt sourceMaxLength, FInt* pDestination);
bool FWideStringConvertToUnsignedLong(const wchar* pSource, FInt sourceMaxLength, uint3232* pDestination);
bool FWideStringConvertToUnsignedLongLong(const wchar* pSource, FInt sourceMaxLength, uint3264* pDestination);
*/

#endif