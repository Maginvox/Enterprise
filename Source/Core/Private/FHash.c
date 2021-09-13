#include "Core/FMacro.h"
#include "Core/FString.h"
#include "Core/FHash.h"

/* ====================================================== */
FUInt32 FHashMultiplicationMethod(const char* pString)
{
    static const FInt64 MULTIPLIER = 37;
    FUInt32 hash = 0;
    unsigned const char* us; 

    FInt32 stringLength = FStringLength(pString, ENTERPRISE_NAME_MAX_LENGTH);

    /* cast s to unsigned const char * */
    /* this ensures that elements of s will be treated as having values >= 0 */
    us = (unsigned const char *) pString;

    hash = 0;
    FInt32 index = 0;
    while(*us != '\0' && index < stringLength) {
        hash = hash * MULTIPLIER + *us;
        us++;
        index++;
    } 

    return hash;
}