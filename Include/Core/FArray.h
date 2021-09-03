#ifndef __FARRAY_H__
#define __FARRAY_H__

#include "FTypes.h"

typedef struct FArray
{
    FInt64 elementSize;
    FInt32 count;
    FInt32 capacity;
    void* pArray;
} FArray;

FArray* FArrayCreate(FInt64 elementSize, FInt32 capacity);
void FArrayDestroy(FArray** ppArray);
bool FArrayResize(FArray* pArray, FInt32 newCapacity); /* Can only resize not filled element slots */
bool FArrayAdd(FArray* pArray, const void* pValue);
bool FArrayRemove(FArray* pArray, FInt32 index);
bool FArraySet(FArray* pArray, FInt32 index, const void* pValue);
void* FArrayGet(FArray* pArray, FInt32 index);
bool FArrayGetCopy(FArray* pArray, FInt32 index, void* pValue);
void* FArrayData(FArray* pArray);
FInt32 FArrayCount(FArray* pArray);

#endif