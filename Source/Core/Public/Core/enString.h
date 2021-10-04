/* Copyright © 2021 Caden Miller, All Rights Reserved. */

#ifndef __FSTRING_H__
#define __FSTRING_H__

#include "Core/enTypes.h"
#include "Core/FTime.h"

#define FSTRING_MAX_LENGTH 1024

typedef enum enStringFormatType
{
    enStringFormatType_Int = 0,
    enStringFormatType_Float,
    enStringFormatType_Double,
    enStringFormatType_String,
    enStringFormatType_Max = enStringFormatType_String
} enStringFormatType;

typedef struct enStringFormatArgument
{
    enStringFormatType type;
    union
    {
        int64 Integer;
        double Double;
        float Float;
        char* String;
    } value;
} enStringFormatArgument;

bool enCharacterIsSpace(char Character);
bool enCharacterIsDigit(char Character);
bool enCharacterIsAlpha(char Character);
bool enCharacterIsAlphaNumeric(char Character);

uint32 enStringLength(const char* pString, uint32 maxLength);
char* enStringSeperate(const char* pSource, uint32 sourceMaxLength, const char* pSeparators, uint32 separatorsLength);
bool enStringConcatenate(const char* pSource, uint32 sourceMaxLength, char* pDestination, uint32 destinationMaxLength);
uint32 enStringCompare(const char* pSource, uint32 sourceMaxLength, const char* pCompare, uint32 compareMaxLength);
bool enStringCopy(const char* pSource, uint32 sourceMaxLength, char* pDestination, uint32 destinationMaxLength);
bool enStringReverse(char* pString, uint32 maxLength);
void enStringRemoveSpaces(char* pString);
bool enStringIsAlpha(const char* pString);
bool enStringIsNumeric(const char* pString);
bool enStringIsAlphaNumeric(const char* pString);
bool enStringFormat(char* pDestination, uint32 destinationMaxLength, const char* pFormat, const enStringFormatArgument* pFormatArguments, uint32 formatArgumentsCount); /* Possible escape formats, %i for int, %f for float, %d for double, and %s for string */

bool enStringConvertFromInt(int64 value, int8 base, char* pDestination, uint32 destinationMaxLength);
bool enStringConvertFromFloat(float value, char* pDestination, uint32 destinationMaxLength);
bool enStringConvertFromDouble(double value, char* pDestination, uint32 destinationMaxLength);
bool enStringConvertFromBool(bool value, char* pDestination, uint32 destinationMaxLength);
bool enStringConvertFromTime(const enTime* pValue, char* pDestination, uint32 destinationMaxLength); /* Destination must be greater than or equal to 32 characters */

bool enStringConvertToInt(const char* pSource, uint32 sourceMaxLength, int64* pValue);
bool enStringConvertToFloat(const char* pSource, uint32 sourceMaxLength, float* pValue);
bool enStringConvertToDouble(const char* pSource, uint32 sourceMaxLength, double* pValue);
bool enStringConvertToBool(const char* pSource, uint32 sourceMaxLength, bool* pValue);

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