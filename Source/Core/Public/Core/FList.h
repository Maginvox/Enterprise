#ifndef __FLIST_H__
#define __FLIST_H__

#include "FTypes.h"

typedef struct FListNode
{
    void* pData;
    struct FListNode* pPrev;
    struct FListNode* pNext;
} FListNode;

typedef struct FList
{
    FListNode* pFirst;
    FListNode* pLast;
    FInt64 count;
} FList;

FEXPORT FList* FListCreate();
FEXPORT void FListDestroy(FList** ppList);
FEXPORT bool FListAdd(FList* pList, void* pData, FListNode** ppNode);
FEXPORT void FListRemove(FList* pList, FInt64 index);
FEXPORT void FListRemoveNode(FList* pList, FListNode* pNode);
FEXPORT void* FListGet(FList* pList, FInt64 index);
FEXPORT bool FListEnumerate(FList* pList, FListNode** pNode);

#endif