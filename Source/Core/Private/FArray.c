#include "Core/FMemory.h"
#include "Core/FArray.h"

FArray* FArrayCreate(FInt64 elementSize, FInt32 capacity)
{
    if (elementSize < 1 || capacity < 1)
    {
        return NULL;
    }
    
    FArray* pArray = FAllocateZero(1, sizeof(FArray));
    if (pArray == NULL)
    {
        return NULL;
    }
    
    pArray->elementSize = elementSize;
    
    if (!FArrayResize(pArray, capacity))
    {
        FArrayDestroy(&pArray);
        return NULL;
    }

    return pArray;
}

void FArrayDestroy(FArray** ppArray)
{
    if (ppArray == NULL || *ppArray == NULL)
    {
        return;
    }

    FArray* pArray = *ppArray;

    if (pArray != NULL)
    {
        FDeallocate(pArray->pArray);
    }

    FDeallocate(pArray);
    *ppArray = NULL;
}

bool FArrayResize(FArray* pArray, FInt32 newCapacity)
{
    if (pArray == NULL || newCapacity < pArray->count) /* You cannot remove elements using this function */
    {
        return false; 
    }

    /* Reallocate the array to be proper size */
    void* pNewArray = FReallocate(pArray->pArray, pArray->elementSize * newCapacity);
    if (pNewArray == NULL)
    {
        return false;
    }

    pArray->capacity = newCapacity;
    pArray->pArray = pNewArray;
    
    return true;
}

bool FArrayAdd(FArray* pArray, const void* pData)
{
    if (pArray == NULL)
    {
        return false;
    }

    if (pArray->capacity == pArray->count)
    {
        FArrayResize(pArray, pArray->capacity + 5);
    }

    if (pData != NULL)
    {
        FMemoryCopy(pData, &((unsigned char*)pArray->pArray)[pArray->count * pArray->elementSize], pArray->elementSize);
    }
    else
    {
        FMemorySet(&((unsigned char*)pArray->pArray)[pArray->count * pArray->elementSize], pArray->elementSize, 0);
    }

    return true;
}

bool FArrayRemove(FArray* pArray, FInt32 index)
{
    if (pArray == NULL || index < 0 || index >= pArray->count)
    {
        return false;
    }

    /* Find the index */
    unsigned char* pArrayAtIndex = &((unsigned char*)pArray->pArray)[pArray->elementSize * index];
    
    pArray->count--;

    if (index != (pArray->count - 1)) /* If this is not the last index */
    {
        unsigned char* pArrayAtNextIndex = pArrayAtIndex + pArray->elementSize;
        FMemoryCopy(pArrayAtNextIndex, pArrayAtIndex, ((pArray->count-1) - index) * pArray->elementSize);
    }

    return true;
}

bool FArraySet(FArray* pArray, FInt32 index, const void* pValue)
{
    if (pArray == NULL || index < 0 || index >= pArray->count)
    {
        return false;
    }

    unsigned char* pArrayAtIndex = &((unsigned char*)pArray->pArray)[pArray->elementSize * index];
    
    return FMemoryCopy(pValue, pArrayAtIndex, pArray->elementSize) != NULL;
}

void* FArrayGet(FArray* pArray, FInt32 index)
{
    if (pArray == NULL || index < 0 || index >= pArray->count)
    {
        return false;
    }

    /* Convert the array to an unsigned char* so we can retrieve a certain offset of bytes */
    return ((unsigned char*)pArray->pArray) + (pArray->elementSize * index);
}

bool FArrayGetCopy(FArray* pArray, FInt32 index, void* pValue)
{
    if (pArray == NULL || index < 0 || index >= pArray->count)
    {
        return false;
    }

    void* pElement = FArrayGet(pArray, index);

    return pElement == NULL ? 
        false : FMemoryCopy(pElement, pValue, pArray->elementSize) != NULL;
}

void* FArrayData(FArray* pArray)
{
    return pArray == NULL ? NULL : pArray->pArray;
}

FInt32 FArrayCount(FArray* pArray)
{
    return pArray == NULL ? 0 : pArray->count;
}