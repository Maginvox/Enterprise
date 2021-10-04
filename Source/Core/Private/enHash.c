/* Copyright © 2021 Caden Miller, All Rights Reserved. */

#include "Core/enMacro.h"
#include "Core/enString.h"
#include "Core/enHash.h"

/* ====================================================== */
uint32 enHashMultiplicationMethod(const char* pString)
{
    static const int32 MULTIPLIER = 37;
    uint32 hash = 0;
    unsigned const char* us; 

    int32 stringLength = enStringLength(pString, ENTERPRISE_NAME_MAX_LENGTH);

    /* cast s to unsigned const char * */
    /* this ensures that elements of s will be treated as having values >= 0 */
    us = (unsigned const char *) pString;

    hash = 0;
    int32 index = 0;
    while(*us != '\0' && index < stringLength) {
        hash = hash * MULTIPLIER + *us;
        us++;
        index++;
    } 

    return hash;
}