#ifndef __FHASH_MAP_H__
#define __FHASH_MAP_H__

#include "FTypes.h"

typedef struct FHashMap
{
    void** ppValues;
    FInt64 size;
    FInt64 count;
} FHashMap;

FEXPORT FHashMap* FHashMapCreate(FInt64 size);
FEXPORT void FHashMapDestroy(FHashMap** ppHashMapSimple);
FEXPORT bool FHashMapInsert(FHashMap* pHashMapSimple, const char* pKey, void* pValue);
FEXPORT bool FHashMapInsertWithHashedKey(FHashMap* pHashMapSimple, const FInt64 hashedKey, void* pValue); /* Use the FHashMultiplicationMethod */
FEXPORT void FHashMapRemove(FHashMap* pHashMapSimple, const char* pKey);
FEXPORT void FHashMapRemoveWithHashedKey(FHashMap* pHashMapSimple, const FInt64 hashedKey);
FEXPORT void FHashMapRemoveWithIndex(FHashMap* pHashMapSimple, const FInt64 index);
FEXPORT bool FHashMapContains(FHashMap* pHashMapSimple, const char* pKey);
FEXPORT bool FHashMapContainsWithHashedKey(FHashMap* pHashMapSimple, const FInt64 hashedKey);
FEXPORT void* FHashMapGet(FHashMap* pHashMapSimple, const char* pKey);
FEXPORT void* FHashMapGetWithHashedKey(FHashMap* pHashMapSimple, const FInt64 hashedKey);
FEXPORT bool FHashMapEnumerate(FHashMap* pHashMapSimple, FInt64* pIndex, void** ppValue);

#endif