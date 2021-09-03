#include <stdlib.h>

#include "Core/FMemory.h"

/* ====================================================== */
void* FAllocate(FUInt64 size)
{
    return malloc(size);
}

/* ====================================================== */
void* FAllocateZero(FUInt64 count, FUInt64 size)
{
    return calloc(count, size);
}

/* ====================================================== */
void* FReallocate(void* pMemory, FUInt64 size)
{
    return realloc(pMemory, size);
}

/* ====================================================== */
void FDeallocate(void* pMemory)
{
    free(pMemory);
}

/* ====================================================== */
void* FMemoryCopy(const void* pSource, void* pDestination, FInt64 size) /* TODO: Make size a constant */
{
    if (pSource == NULL || pDestination == NULL || size <= 0)
    {
        return NULL;
    }
    
    char* pTemp = (char*)pDestination;
    const char* pS = (const char*)pSource;
    while (size--)
    {
        *pTemp++ = *pS++;
    }

    return pDestination;
}

/* ====================================================== */
void* FMemorySet(void* pDestination, FInt64 size, FInt value)
{
    if (pDestination == NULL || size <= 0)
    {
        return NULL;
    }

    unsigned char* pTemporary = pDestination;

    while (size--)
    {
        *pTemporary++ = (unsigned char)value;
    }

    return pDestination;
}