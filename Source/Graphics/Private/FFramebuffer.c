/* Copyright © 2021 Caden Miller, All Rights Reserved. */

#include "FGraphicsContext_Impl.h"
#include "Graphics/FFramebuffer.h"

FFramebuffer* FFramebufferCreate(FGraphicsContext* pContext, const FFramebufferCreateInfo* pInfo)
{
    
    if (pContext == NULL || pInfo == NULL)
    {
        return NULL;
    }

    return pContext->pFramebufferCreate(pContext, pInfo);
}

void FFramebufferDestroy(FGraphicsContext* pContext, FFramebuffer** ppFramebuffer)
{

}