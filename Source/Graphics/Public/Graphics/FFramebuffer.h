/* Copyright © 2021 Caden Miller, All Rights Reserved. */

#ifndef __FFRAMEBUFFER_H__
#define __FFRAMEBUFFER_H__

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

typedef FFramebuffer* (*FFramebufferCreateFunction)(const FFramebufferCreateInfo* pInfo);
typedef void (*FFramebufferDestroyFunction)(FFramebuffer** ppFramebuffer);

FFramebuffer* FFramebufferCreate(const FFramebufferCreateInfo* pInfo);
void FFramebufferDestroy(FFramebuffer** ppFramebuffer);

#endif