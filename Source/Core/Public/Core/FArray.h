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

FEXPORT FArray* FArrayCreate(FInt64 elementSize, FInt32 capacity);
FEXPORT void FArrayDestroy(FArray** ppArray);
FEXPORT bool FArrayResize(FArray* pArray, FInt32 newCapacity); /* Can only resize not filled element slots */
FEXPORT bool FArrayAdd(FArray* pArray, const void* pValue);
FEXPORT bool FArrayRemove(FArray* pArray, FInt32 index);
FEXPORT bool FArraySet(FArray* pArray, FInt32 index, const void* pValue);
FEXPORT void* FArrayGet(FArray* pArray, FInt32 index);
FEXPORT bool FArrayGetCopy(FArray* pArray, FInt32 index, void* pValue);
FEXPORT void* FArrayData(FArray* pArray);
FEXPORT FInt32 FArrayCount(FArray* pArray);

#endif