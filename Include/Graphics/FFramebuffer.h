/*
   Notice: Copyright © 2021 Caden Miller, All Rights Reserved.
 */

#ifndef __FFRAMEBUFFER_H__
#define __FFRAMEBUFFER_H__

#include "FImage.h"
#include "FRenderPass.h"

typedef struct FFramebufferCreateInfo
{
    FRenderPass* pRenderPass;
    FInt32 attachmentsCount;
    FImage** pAttachments;
    FInt32 width;
    FInt32 height;
    FInt32 depth;
} FFramebufferCreateInfo;

typedef struct FFramebuffer FFramebuffer;

typedef FFramebuffer* (*PFN_FFramebufferCreate)(const FFramebufferCreateInfo* pInfo);
typedef void (*PFN_FFramebufferDestroy)(FFramebuffer** ppFramebuffer);

#endif