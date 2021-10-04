/* Copyright © 2021 Caden Miller, All Rights Reserved. */

#include "Core/enMacro.h"
#include "Core/enMemory.h"
#include "Core/enString.h"
#include "Core/enHash.h"
#include "Core/enHashMap.h"

enHashMap* enHashMapCreate(int64 size)
{
    if (size <= 0)
    {
        return NULL;
    }

    enHashMap* pHashMap = enAllocateZero(1, sizeof(enHashMap));
    if (pHashMap == NULL)
    {
        return NULL;
    } 

    pHashMap->size = size;
    pHashMap->count = 0;

    pHashMap->ppValues = enAllocateZero(size, sizeof(void*));
    if (pHashMap->ppValues == NULL)
    {
        enHashMapDestroy(&pHashMap);
        return NULL;
    }

    return pHashMap;
}

void enHashMapDestroy(enHashMap** ppHashMap)
{
    if (ppHashMap == NULL || *ppHashMap == NULL)
    {
        return;
    }

    enHashMap* pHashMap = *ppHashMap;

    if (pHashMap->ppValues != NULL)
    {
        enDeallocate(pHashMap->ppValues);
        pHashMap->ppValues = NULL;
    }

    enDeallocate(pHashMap);
    *ppHashMap = NULL;
}

bool enHashMapInsert(enHashMap* pHashMapSimple, const char* pKey, void* pValue)
{
    if (pHashMapSimple == NULL || pKey == NULL)
    {
        return false;
    }

    /* Compute the hash, get the index and set the value */
    int64 hash = enHashMultiplicationMethod(pKey);
    int64 index = hash % pHashMapSimple->size;
    pHashMapSimple->ppValues[index] = pValue;
    
    return true;
}

bool enHashMapInsertWithHashedKey(enHashMap* pHashMapSimple, const int64 hashedKey, void* pValue)
{
    if (pHashMapSimple == NULL || hashedKey < 0)
    {
        return false;
    }

    /* Get the index and set the value */
    int64 index = hashedKey % pHashMapSimple->size;
    pHashMapSimple->ppValues[index] = pValue;
    
    return true;
}

void enHashMapRemove(enHashMap* pHashMapSimple, const char* pKey)
{
    if (pHashMapSimple == NULL || pKey == NULL)
    {
        return;
    }

    int64 hash = enHashMultiplicationMethod(pKey);
    int64 index = hash % pHashMapSimple->size;
    pHashMapSimple->ppValues[index] = NULL;
}

void enHashMapRemoveWithHashedKey(enHashMap* pHashMapSimple, const int64 hashedKey)
{
    if (pHashMapSimple == NULL || hashedKey < 0)
    {
        return;
    }

    int64 index = hashedKey % pHashMapSimple->size;
    pHashMapSimple->ppValues[index] = NULL;
}

void enHashMapRemoveWithIndex(enHashMap* pHashMapSimple, const int64 index)
{
    if (pHashMapSimple == NULL || index < 0)
    {
        return;
    }

    pHashMapSimple->ppValues[index] = NULL;
}

bool enHashMapContains(enHashMap* pHashMapSimple, const char* pKey)
{
    if (pHashMapSimple == NULL || pKey == NULL)
    {
        return false;
    }

    return enHashMapContainsWithHashedKey(pHashMapSimple, enHashMultiplicationMethod(pKey));
}

bool enHashMapContainsWithHashedKey(enHashMap* pHashMapSimple, const int64 hashedKey)
{
    if (pHashMapSimple == NULL)
    {
        return false;
    }

    int64 index = hashedKey % pHashMapSimple->size;
    return (pHashMapSimple->ppValues[index] != NULL);
}

void* enHashMapGet(enHashMap* pHashMapSimple, const char* pKey)
{
    if (pHashMapSimple == NULL || pKey == NULL)
    {
        return NULL;
    }

    int64 hash = enHashMultiplicationMethod(pKey);
    int64 index = hash % pHashMapSimple->size;
    return pHashMapSimple->ppValues[index];
}

void* enHashMapGetWithHashedKey(enHashMap* pHashMapSimple, const int64 hashedKey)
{
    if (pHashMapSimple == NULL)
    {
        return NULL;
    }

    int64 index = hashedKey % pHashMapSimple->size;
    return pHashMapSimple->ppValues[index];
}

bool enHashMapEnumerate(enHashMap* pHashMapSimple, int64* pIndex, void** ppValue)
{
    if (pHashMapSimple == NULL || pIndex == NULL || *pIndex < 0 || *pIndex >= pHashMapSimple->size)
    {
        return false;
    }

    *ppValue = pHashMapSimple->ppValues[++*pIndex];
    return true;
}