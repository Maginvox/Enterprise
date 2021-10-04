/* Copyright © 2021 Caden Miller, All Rights Reserved. */

#include "Core/enMemory.h"
#include "Core/enArray.h"

enArray* enArrayCreate(int64 elementSize, int32 capacity)
{
    if (elementSize < 1 || capacity < 1)
    {
        return NULL;
    }
    
    enArray* pArray = enAllocateZero(1, sizeof(enArray));
    if (pArray == NULL)
    {
        return NULL;
    }
    
    pArray->elementSize = elementSize;
    
    if (!enArrayResize(pArray, capacity))
    {
        enArrayDestroy(&pArray);
        return NULL;
    }

    return pArray;
}

void enArrayDestroy(enArray** ppArray)
{
    if (ppArray == NULL || *ppArray == NULL)
    {
        return;
    }

    enArray* pArray = *ppArray;

    if (pArray != NULL)
    {
        enDeallocate(pArray->pArray);
    }

    enDeallocate(pArray);
    *ppArray = NULL;
}

bool enArrayResize(enArray* pArray, int32 newCapacity)
{
    if (pArray == NULL || newCapacity < pArray->count) /* You cannot remove elements using this function */
    {
        return false; 
    }

    /* Reallocate the array to be proper size */
    void* pNewArray = enReallocate(pArray->pArray, pArray->elementSize * newCapacity);
    if (pNewArray == NULL)
    {
        return false;
    }

    pArray->capacity = newCapacity;
    pArray->pArray = pNewArray;
    
    return true;
}

bool enArrayAdd(enArray* pArray, const void* pData)
{
    if (pArray == NULL)
    {
        return false;
    }

    if (pArray->capacity == pArray->count)
    {
        enArrayResize(pArray, pArray->capacity + 5);
    }

    if (pData != NULL)
    {
        enMemoryCopy(pData, &((unsigned char*)pArray->pArray)[pArray->count * pArray->elementSize], pArray->elementSize);
    }
    else
    {
        enMemorySet(&((unsigned char*)pArray->pArray)[pArray->count * pArray->elementSize], pArray->elementSize, 0);
    }

    return true;
}

bool enArrayRemove(enArray* pArray, int32 index)
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
        enMemoryCopy(pArrayAtNextIndex, pArrayAtIndex, ((pArray->count-1) - index) * pArray->elementSize);
    }

    return true;
}

bool enArraySet(enArray* pArray, int32 index, const void* pValue)
{
    if (pArray == NULL || index < 0 || index >= pArray->count)
    {
        return false;
    }

    unsigned char* pArrayAtIndex = &((unsigned char*)pArray->pArray)[pArray->elementSize * index];
    
    return enMemoryCopy(pValue, pArrayAtIndex, pArray->elementSize) != NULL;
}

void* enArrayGet(enArray* pArray, int32 index)
{
    if (pArray == NULL || index < 0 || index >= pArray->count)
    {
        return false;
    }

    /* Convert the array to an unsigned char* so we can retrieve a certain offset of bytes */
    return ((unsigned char*)pArray->pArray) + (pArray->elementSize * index);
}

bool enArrayGetCopy(enArray* pArray, int32 index, void* pValue)
{
    if (pArray == NULL || index < 0 || index >= pArray->count)
    {
        return false;
    }

    void* pElement = enArrayGet(pArray, index);

    return pElement == NULL ? 
        false : enMemoryCopy(pElement, pValue, pArray->elementSize) != NULL;
}

void* enArrayData(enArray* pArray)
{
    return pArray == NULL ? NULL : pArray->pArray;
}

int32 enArrayCount(enArray* pArray)
{
    return pArray == NULL ? 0 : pArray->count;
}