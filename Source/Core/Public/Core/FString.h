#ifndef __FSTRING_H__
#define __FSTRING_H__

#include "Core/FExport.h"
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
        FInt Integer;
        double Double;
        float Float;
        char* String;
    } value;
} FStringFormatArgument;

FEXPORT bool FCharacterIsSpace(char Character);
FEXPORT bool FCharacterIsDigit(char Character);
FEXPORT bool FCharacterIsAlpha(char Character);
FEXPORT bool FCharacterIsAlphaNumeric(char Character);

FEXPORT FInt FStringLength(const char* pString, FInt maxLength);
FEXPORT char* FStringSeperate(const char* pSource, FInt sourceMaxLength, const char* pSeparators, FInt separatorsLength);
FEXPORT bool FStringConcatenate(const char* pSource, FInt sourceMaxLength, char* pDestination, FInt destinationMaxLength);
FEXPORT FInt FStringCompare(const char* pSource, FInt sourceMaxLength, const char* pCompare, FInt compareMaxLength);
FEXPORT bool FStringCopy(const char* pSource, FInt sourceMaxLength, char* pDestination, FInt destinationMaxLength);
FEXPORT bool FStringReverse(char* pString, FInt maxLength);
FEXPORT void FStringRemoveSpaces(char* pString);
FEXPORT bool FStringIsAlpha(const char* pString);
FEXPORT bool FStringIsNumeric(const char* pString);
FEXPORT bool FStringIsAlphaNumeric(const char* pString);
FEXPORT bool FStringFormat(char* pDestination, FInt destinationMaxLength, const char* pFormat, const FStringFormatArgument* pFormatArguments, FInt formatArgumentsCount); /* Possible escape formats, %i for int, %f for float, %d for double, and %s  */

FEXPORT bool FStringConvertFromInt(FInt value, FInt8 base, char* pDestination, FInt destinationMaxLength);
FEXPORT bool FStringConvertFromFloat(float value, char* pDestination, FInt destinationMaxLength);
FEXPORT bool FStringConvertFromDouble(double value, char* pDestination, FInt destinationMaxLength);
FEXPORT bool FStringConvertFromBool(bool value, char* pDestination, FInt destinationMaxLength);
FEXPORT bool FStringConvertFromTime(const FTime* pValue, char* pDestination, FInt destinationMaxLength); /* Destination must be greater than or equal to 32 characters */

FEXPORT bool FStringConvertToInt(const char* pSource, FInt sourceMaxLength, FInt* pValue);
FEXPORT bool FStringConvertToFloat(const char* pSource, FInt sourceMaxLength, float* pValue);
FEXPORT bool FStringConvertToDouble(const char* pSource, FInt sourceMaxLength, double* pValue);
FEXPORT bool FStringConvertToBool(const char* pSource, FInt sourceMaxLength, bool* pValue);



/*
FInt FWideStringLength(const FWChar* pString, FInt MaxLength);
FWChar* FWideStringTokenize(const FWChar* pSource, FInt* pSourceMaxLength, const FWChar* pDeliminator, FWChar** ppToken);
bool FWideStringConcatenate(const FWChar* pSource, FInt sourceMaxLength, FWChar* pDestination, FInt destinationMaxLength);
bool FWideStringCompare(const FWChar* pSource, FInt sourceMaxLength, const FWChar* pCompare, FInt compareMaxLength);
bool FWideStringCopy(const FWChar* pSource, FInt sourceMaxLength, FWChar* pDestination, FInt destinationMaxLength);

bool FWideStringConvertFromBool(const bool value, FWChar* pDestination, FInt destinationMaxLength);
bool FWideStringConvertFromLong(const FInt32 value, FWChar* pDestination, FInt destinationMaxLength);
bool FWideStringConvertFromLongLong(const FInt value, FWChar* pDestination, FInt destinationMaxLength);
bool FWideStringConvertFromUnsignedLong(const FUInt32 value, FWChar* pDestination, FInt destinationMaxLength);
bool FWideStringConvertFromUnsignedLongLong(const FInt value, FWChar* pDestination, FInt destinationMaxLength);
bool FWideStringConvertToBool(const FWChar* pSource, FInt sourceMaxLength, bool *pDestination);
bool FWideStringConvertToLong(const FWChar* pSource, FInt sourceMaxLength, FInt32* pDestination);
bool FWideStringConvertToLongLong(const FWChar* pSource, FInt sourceMaxLength, FInt* pDestination);
bool FWideStringConvertToUnsignedLong(const FWChar* pSource, FInt sourceMaxLength, FUInt32* pDestination);
bool FWideStringConvertToUnsignedLongLong(const FWChar* pSource, FInt sourceMaxLength, FUInt64* pDestination);
*/

#endif