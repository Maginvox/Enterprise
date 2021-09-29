/* Copyright © 2021 Caden Miller, All Rights Reserved. */

#ifndef __FRENDERPASS_H__
#define __FRENDERPASS_H__

#include "Core/FExport.h"
#include "Core/FTypes.h"
#include "Graphics/FImage.h"

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
    FRenderPassAttachmentLoadOp loadOp;
    FRenderPassAttachmentStoreOp storeOp;
    FRenderPassAttachmentLoadOp stencilLoadOp;
    FRenderPassAttachmentStoreOp stencilStoreOp;
} FRenderPassAttachment;

/* FRenderPassCreateInfo
    A render pass only contains one depth/stencil attachment and multiple color attachments. */
typedef struct FRenderPassCreateInfo
{
    FInt32 colorAttachmentsCount;
    const FRenderPassAttachment* pColorAttachments;
    const FRenderPassAttachment* pDepthStencilAttachment;
} FRenderPassCreateInfo;

typedef void* FRenderPass;

FRenderPass FRenderPassCreate(const FRenderPassCreateInfo* pInfo);
void FRenderPassDestroy(FRenderPass renderPass);

#endif