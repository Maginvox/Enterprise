#include "Core/FMacro.h"
#include "Core/FMemory.h"
#include "Core/FString.h"
#include "Core/FHash.h"
#include "Core/FHashMap.h"

FHashMap* FHashMapCreate(FInt64 size)
{
    if (size <= 0)
    {
        return NULL;
    }

    FHashMap* pHashMap = FAllocateZero(1, sizeof(FHashMap));
    if (pHashMap == NULL)
    {
        return NULL;
    } 

    pHashMap->size = size;
    pHashMap->count = 0;

    pHashMap->ppValues = FAllocateZero(size, sizeof(void*));
    if (pHashMap->ppValues == NULL)
    {
        FHashMapDestroy(&pHashMap);
        return NULL;
    }

    return pHashMap;
}

void FHashMapDestroy(FHashMap** ppHashMap)
{
    if (ppHashMap == NULL || *ppHashMap == NULL)
    {
        return;
    }

    FHashMap* pHashMap = *ppHashMap;

    if (pHashMap->ppValues != NULL)
    {
        FDeallocate(pHashMap->ppValues);
        pHashMap->ppValues = NULL;
    }

    FDeallocate(pHashMap);
    *ppHashMap = NULL;
}

bool FHashMapInsert(FHashMap* pHashMapSimple, const char* pKey, void* pValue)
{
    if (pHashMapSimple == NULL || pKey == NULL)
    {
        return false;
    }

    /* Compute the hash, get the index and set the value */
    FInt64 hash = FHashMultiplicationMethod(pKey);
    FInt64 index = hash % pHashMapSimple->size;
    pHashMapSimple->ppValues[index] = pValue;
    
    return true;
}

bool FHashMapInsertWithHashedKey(FHashMap* pHashMapSimple, const FInt64 hashedKey, void* pValue)
{
    if (pHashMapSimple == NULL || hashedKey < 0)
    {
        return false;
    }

    /* Get the index and set the value */
    FInt64 index = hashedKey % pHashMapSimple->size;
    pHashMapSimple->ppValues[index] = pValue;
    
    return true;
}

void FHashMapRemove(FHashMap* pHashMapSimple, const char* pKey)
{
    if (pHashMapSimple == NULL || pKey == NULL)
    {
        return;
    }

    FInt64 hash = FHashMultiplicationMethod(pKey);
    FInt64 index = hash % pHashMapSimple->size;
    pHashMapSimple->ppValues[index] = NULL;
}

void FHashMapRemoveWithHashedKey(FHashMap* pHashMapSimple, const FInt64 hashedKey)
{
    if (pHashMapSimple == NULL || hashedKey < 0)
    {
        return;
    }

    FInt64 index = hashedKey % pHashMapSimple->size;
    pHashMapSimple->ppValues[index] = NULL;
}

void FHashMapRemoveWithIndex(FHashMap* pHashMapSimple, const FInt64 index)
{
    if (pHashMapSimple == NULL || index < 0)
    {
        return;
    }

    pHashMapSimple->ppValues[index] = NULL;
}

bool FHashMapContains(FHashMap* pHashMapSimple, const char* pKey)
{
    if (pHashMapSimple == NULL || pKey == NULL)
    {
        return false;
    }

    return FHashMapContainsWithHashedKey(pHashMapSimple, FHashMultiplicationMethod(pKey));
}

bool FHashMapContainsWithHashedKey(FHashMap* pHashMapSimple, const FInt64 hashedKey)
{
    if (pHashMapSimple == NULL)
    {
        return false;
    }

    FInt64 index = hashedKey % pHashMapSimple->size;
    return (pHashMapSimple->ppValues[index] != NULL);
}

void* FHashMapGet(FHashMap* pHashMapSimple, const char* pKey)
{
    if (pHashMapSimple == NULL || pKey == NULL)
    {
        return NULL;
    }

    FInt64 hash = FHashMultiplicationMethod(pKey);
    FInt64 index = hash % pHashMapSimple->size;
    return pHashMapSimple->ppValues[index];
}

void* FHashMapGetWithHashedKey(FHashMap* pHashMapSimple, const FInt64 hashedKey)
{
    if (pHashMapSimple == NULL)
    {
        return NULL;
    }

    FInt64 index = hashedKey % pHashMapSimple->size;
    return pHashMapSimple->ppValues[index];
}

bool FHashMapEnumerate(FHashMap* pHashMapSimple, FInt64* pIndex, void** ppValue)
{
    if (pHashMapSimple == NULL || pIndex == NULL || *pIndex < 0 || *pIndex >= pHashMapSimple->size)
    {
        return false;
    }

    *ppValue = pHashMapSimple->ppValues[++*pIndex];
    return true;
}