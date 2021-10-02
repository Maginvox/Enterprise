/* Copyright © 2021 Caden Miller, All Rights Reserved. */

#ifndef __FHASH_MAP_H__
#define __FHASH_MAP_H__

#include "FTypes.h"

typedef struct FHashMap
{
    void** ppValues;
    int64 size;
    int64 count;
} FHashMap;

FHashMap* FHashMapCreate(int64 size);
void FHashMapDestroy(FHashMap** ppHashMapSimple);
bool FHashMapInsert(FHashMap* pHashMapSimple, const char* pKey, void* pValue);
bool FHashMapInsertWithHashedKey(FHashMap* pHashMapSimple, const int64 hashedKey, void* pValue); /* Use the FHashMultiplicationMethod */
void FHashMapRemove(FHashMap* pHashMapSimple, const char* pKey);
void FHashMapRemoveWithHashedKey(FHashMap* pHashMapSimple, const int64 hashedKey);
void FHashMapRemoveWithIndex(FHashMap* pHashMapSimple, const int64 index);
bool FHashMapContains(FHashMap* pHashMapSimple, const char* pKey);
bool FHashMapContainsWithHashedKey(FHashMap* pHashMapSimple, const int64 hashedKey);
void* FHashMapGet(FHashMap* pHashMapSimple, const char* pKey);
void* FHashMapGetWithHashedKey(FHashMap* pHashMapSimple, const int64 hashedKey);
bool FHashMapEnumerate(FHashMap* pHashMapSimple, int64* pIndex, void** ppValue);

#endif