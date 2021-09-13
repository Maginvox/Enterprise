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

FList* FListCreate();
void FListDestroy(FList** ppList);
bool FListAdd(FList* pList, void* pData, FListNode** ppNode);
/* void FListRemove(FList* pList, FInt64 index); */
void FListRemoveNode(FList* pList, FListNode* pNode);
void* FListGet(FList* pList, FInt64 index);
bool FListEnumerate(FList* pList, FListNode** pNode);

#endif