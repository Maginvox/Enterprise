/*
    : Copyright © 2021 Caden Miller, All Rights Reserved.
 */

#ifndef __FRENDERPASS_H__
#define __FRENDERPASS_H__

#include "FTypes.h"
#include "FImage.h"

typedef enum FRenderPassAttachmentLoadOp
{
    FRenderPassAttachmentLoadOp_Load,
    FRenderPassAttachmentLoadOp_DontCare,
    FRenderPassAttachmentLoadOp_Clear,
} FRenderPassAttachmentLoadOp;

typedef enum FRenderPassAttachmentStoreOp
{
    FRenderPassAttachmentStoreOp_DontCare,
    FRenderPassAttachmentStoreOp_Store,
} FRenderPassAttachmentStoreOp;

typedef struct FRenderPassAttachment
{
    FImageFormat format;
    FImageLayout initialLayout;
    FImageLayout finalLayout;
} FRenderPassAttachment;

typedef struct FRenderPassCreateInfo
{
    FInt32 colorAttachmentsCount;
    const FRenderPassAttachment** pColorAttachments;
    const FRenderPassAttachment* pDepthAttachment;
    const FRenderPassAttachment* pStencilAttachment;
} FRenderPassCreateInfo;

typedef struct FRenderPass FRenderPass;

typedef FRenderPass* (*PFN_FRenderPassCreate)(FRenderContext* pContext, const FRenderPassCreateInfo* pInfo);
typedef void (*PFN_FRenderPassDestroy)(FRenderContext* pContext, FRenderPass** ppRenderPass);

FRenderPass* FRenderPassCreate(FRenderContext* pContext, const FRenderPassCreateInfo* pInfo);
void FRenderPassDestroy(FRenderContext* pContext, FRenderPass** ppRenderPass);

#endif