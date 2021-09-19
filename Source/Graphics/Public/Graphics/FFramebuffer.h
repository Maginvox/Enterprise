/* Copyright © 2021 Caden Miller, All Rights Reserved. */

#ifndef __FFRAMEBUFFER_H__
#define __FFRAMEBUFFER_H__

#include "Graphics/FGraphicsContext.h"
#include "Graphics/FImage.h"
#include "Graphics/FRenderPass.h"

typedef struct FFramebufferCreateInfo
{
    FRenderPass* pRenderPass;
    FInt32 attachmentsCount;
    FImage** ppAttachments;
    FInt32 width;
    FInt32 height;
} FFramebufferCreateInfo;

typedef struct FFramebuffer FFramebuffer;

typedef FFramebuffer* (*FFramebufferCreateFunction)(FGraphicsContext* pContext, const FFramebufferCreateInfo* pInfo);
typedef void (*FFramebufferDestroyFunction)(FGraphicsContext* pContext, FFramebuffer** ppFramebuffer);

FFramebuffer* FFramebufferCreate(FGraphicsContext* pContext, const FFramebufferCreateInfo* pInfo);
void FFramebufferDestroy(FGraphicsContext* pContext, FFramebuffer** ppFramebuffer);

#endif