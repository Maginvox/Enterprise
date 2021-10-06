/* Copyright © 2021 Caden Miller, All Rights Reserved. */

#include <stdlib.h>

#include "Core/enMemory.h"

/* ====================================================== */
void* enMalloc(uint64 size)
{
    return malloc(size);
}

/* ====================================================== */
void* enCalloc(uint64 count, uint64 size)
{
    return calloc(count, size);
}

/* ====================================================== */
void* enRealloc(void* pMemory, uint64 size)
{
    return realloc(pMemory, size);
}

/* ====================================================== */
void enFree(void* pMemory)
{
    free(pMemory);
}

/* ====================================================== */
void* enMemoryCopy(void* pDestination, const void* pSource, uint64 size) /* TODO: Make size a constant */
{
    if (pSource == NULL || pDestination == NULL || size <= 0)
    {
        return NULL;
    }
    
    char* pDest = pDestination;
    const char* pSrc = pSource;

    for (uint64 i = 0; i < size; i++)
    {
        *pDest++ = *pSrc++;
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