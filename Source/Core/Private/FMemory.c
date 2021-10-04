/* Copyright © 2021 Caden Miller, All Rights Reserved. */

#include <stdlib.h>

#include "Core/FMemory.h"

/* ====================================================== */
void* enAllocate(uint64 size)
{
    return malloc(size);
}

/* ====================================================== */
void* enAllocateZero(uint64 count, uint64 size)
{
    return calloc(count, size);
}

/* ====================================================== */
void* enReallocate(void* pMemory, uint64 size)
{
    return realloc(pMemory, size);
}

/* ====================================================== */
void enDeallocate(void* pMemory)
{
    free(pMemory);
}

/* ====================================================== */
void* enMemoryCopy(const void* pSource, void* pDestination, uint64 size) /* TODO: Make size a constant */
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
void* enMemorySet(void* pDestination, uint64 size, uint8 value)
{
    if (pDestination == NULL || size <= 0)
    {
        return NULL;
    }

    unsigned char* pTemporary = pDestination;

    while (size--)
    {
        *pTemporary++ = value;
    }

    return pDestination;
}