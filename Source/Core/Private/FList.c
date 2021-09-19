/* Copyright © 2021 Caden Miller, All Rights Reserved. */

#include "Core/FMemory.h"
#include "Core/FList.h"


FList* FListCreate()
{
    FList* pList = FAllocateZero(1, sizeof(FList));
    return pList;
}

void FListDestroy(FList** ppList)
{
    if (ppList == NULL || *ppList == NULL)
    {
        return;
    }

    FList* pList = *ppList;
    
    FListNode* pCurrentNode = NULL;
    while (FListEnumerate(pList, &pCurrentNode))
    {
        FListRemoveNode(pList, pCurrentNode);
    }

    FDeallocate(pList);
    *ppList = NULL;
}

bool FListAdd(FList* pList, void* pData, FListNode** ppNode)
{
    FListNode* pNewNode = FAllocateZero(1, sizeof(FListNode));
    if (pNewNode == NULL)
    {
        return false;
    }

    pNewNode->pData = pData;
    pNewNode->pNext = NULL;

    /* If the list is empty set the first node to this node */
    if (pList->pFirst == NULL)
    {
        pList->pFirst = pNewNode;
    }
    else /* Otherwise, set the next node to this node */
    {
        pList->pLast->pNext = pNewNode;
    }

    pList->pLast = pNewNode;
    
    if (ppNode != NULL)
    {
        *ppNode = pNewNode;
    }
    
    return true;
}

void FListRemoveNode(FList* pList, FListNode* pNode)
{
    if (pList == NULL || pNode == NULL)
    {
        return;
    }

    /* If the node is the first node, set the first node to the next node */
    if (pList->pFirst == pNode)
    {
        pList->pFirst = pNode->pNext;
    }
    else /* Otherwise, set the previous node's next node to the next node */
    {
        pList->pLast->pNext = pNode->pNext;
    }

    /* If the node is the last node, set the last node to the previous node */
    if (pList->pLast == pNode)
    {
        pList->pLast = pNode->pPrev;
    }

    /* Free the node */
    FDeallocate(pNode);
}

void* FListGet(FList* pList, FInt64 index)
{
    if (pList == NULL || index < 0 || index > pList->count)
    {
        return NULL;
    }

    FListNode* pCurrentNode = NULL;
    while(FListEnumerate(pList, &pCurrentNode))
    {
        if (index == 0)
        {
            return pCurrentNode->pData;
        }
        index--;
    }

    return NULL;
}

bool FListEnumerate(FList* pList, FListNode** ppNode)
{
    if (pList == NULL || ppNode == NULL || *ppNode == NULL)
    {
        return false;
    }

    FListNode* pCurrentNode = *ppNode;

    if (pCurrentNode == NULL)
    {
        pCurrentNode = pList->pFirst;
    }
    else if (pCurrentNode->pNext == NULL)
    {
        return false;
    }
    else
    {
        pCurrentNode = pCurrentNode->pNext;
    }

    *ppNode = pCurrentNode;
    return true;
}