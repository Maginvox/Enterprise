/* Copyright © 2021 Caden Miller, All Rights Reserved. */

#ifndef __FARRAY_H__
#define __FARRAY_H__

#include "Core/enTypes.h"

typedef struct enArray
{
    int64 elementSize;
    int32 count;
    int32 capacity;
    void* pArray;
} enArray;

enArray*    enArrayCreate(int64 elementSize, int32 capacity);
void        enArrayDestroy(enArray** ppArray);
bool        enArrayResize(enArray* pArray, int32 newCapacity); /* Can only resize not filled element slots */
bool        enArrayAdd(enArray* pArray, const void* pValue);
bool        enArrayRemove(enArray* pArray, int32 index);
bool        enArraySet(enArray* pArray, int32 index, const void* pValue);
void*       enArrayGet(enArray* pArray, int32 index);
bool        enArrayGetCopy(enArray* pArray, int32 index, void* pValue);
void*       enArrayData(enArray* pArray);
int32       enArrayCount(enArray* pArray);

#endif