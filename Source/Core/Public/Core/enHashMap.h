/* Copyright © 2021 Caden Miller, All Rights Reserved. */

#ifndef __ENTERPRISE_HASH_MAP_H__
#define __ENTERPRISE_HASH_MAP_H__

#include "Core/enTypes.h"

typedef struct
{
    void** ppValues;
    int64 size;
    int64 count;
} enHashMap;

enHashMap* enHashMapCreate(int64 size);
void enHashMapDestroy(enHashMap** ppHashMapSimple);
bool enHashMapInsert(enHashMap* pHashMapSimple, const char* pKey, void* pValue);
bool enHashMapInsertWithHashedKey(enHashMap* pHashMapSimple, const int64 hashedKey, void* pValue); /* Use the enHashMultiplicationMethod */
void enHashMapRemove(enHashMap* pHashMapSimple, const char* pKey);
void enHashMapRemoveWithHashedKey(enHashMap* pHashMapSimple, const int64 hashedKey);
void enHashMapRemoveWithIndex(enHashMap* pHashMapSimple, const int64 index);
bool enHashMapContains(enHashMap* pHashMapSimple, const char* pKey);
bool enHashMapContainsWithHashedKey(enHashMap* pHashMapSimple, const int64 hashedKey);
void* enHashMapGet(enHashMap* pHashMapSimple, const char* pKey);
void* enHashMapGetWithHashedKey(enHashMap* pHashMapSimple, const int64 hashedKey);
bool enHashMapEnumerate(enHashMap* pHashMapSimple, int64* pIndex, void** ppValue);

#endif