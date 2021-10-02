/* Copyright © 2021 Caden Miller, All Rights Reserved. */

#ifndef __FARRAY_H__
#define __FARRAY_H__

#include "FTypes.h"

typedef struct FArray
{
    int64 elementSize;
    int32 count;
    int32 capacity;
    void* pArray;
} FArray;

FArray* FArrayCreate(int64 elementSize, int32 capacity);
void FArrayDestroy(FArray** ppArray);
bool FArrayResize(FArray* pArray, int32 newCapacity); /* Can only resize not filled element slots */
bool FArrayAdd(FArray* pArray, const void* pValue);
bool FArrayRemove(FArray* pArray, int32 index);
bool FArraySet(FArray* pArray, int32 index, const void* pValue);
void* FArrayGet(FArray* pArray, int32 index);
bool FArrayGetCopy(FArray* pArray, int32 index, void* pValue);
void* FArrayData(FArray* pArray);
int32 FArrayCount(FArray* pArray);

#endif