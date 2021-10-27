/* Copyright © 2021 Caden Miller, All Rights Reserved. */

#ifndef __ENTERPRISE_LIST_H__
#define __ENTERPRISE_LIST_H__

#include "Core/enTypes.h"

typedef struct enListNode
{
    void* pData;
    struct enListNode* pPrev;
    struct enListNode* pNext;
} enListNode;

typedef struct enList
{
    enListNode* pFirst;
    enListNode* pLast;
    int64 count;
} enList;

enList*     enListCreate();
void        enListDestroy(enList** ppList);
bool        enListAdd(enList* pList, void* pData, enListNode** ppNode);
void        enListRemove(enList* pList, int64 index);
void        enListRemoveNode(enList* pList, enListNode* pNode);
void*       enListGet(enList* pList, int64 index);
bool        enListEnumerate(enList* pList, enListNode** pNode);

#endif