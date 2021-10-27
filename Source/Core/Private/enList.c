/* Copyright © 2021 Caden Miller, All Rights Reserved. */

#include "Core/enMemory.h"
#include "Core/enList.h"

enList* enListCreate()
{
    enList* pList = enCalloc(1, sizeof(enList));
    return pList;
}

void enListDestroy(enList** ppList)
{
    if (ppList == NULL || *ppList == NULL)
    {
        return;
    }

    enList* pList = *ppList;
    
    enListNode* pCurrentNode = NULL;
    while (enListEnumerate(pList, &pCurrentNode))
    {
        enListRemoveNode(pList, pCurrentNode);
    }

    enFree(pList);
    *ppList = NULL;
}

bool enListAdd(enList* pList, void* pData, enListNode** ppNode)
{
    enListNode* pNewNode = enCalloc(1, sizeof(enListNode));
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

void enListRemoveNode(enList* pList, enListNode* pNode)
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
    enFree(pNode);
}

void* enListGet(enList* pList, int64 index)
{
    if (pList == NULL || index < 0 || index > pList->count)
    {
        return NULL;
    }

    enListNode* pCurrentNode = NULL;
    while(enListEnumerate(pList, &pCurrentNode))
    {
        if (index == 0)
        {
            return pCurrentNode->pData;
        }
        index--;
    }

    return NULL;
}

bool enListEnumerate(enList* pList, enListNode** ppNode)
{
    if (pList == NULL || ppNode == NULL || *ppNode == NULL)
    {
        return false;
    }

    enListNode* pCurrentNode = *ppNode;

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