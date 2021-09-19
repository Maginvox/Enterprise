/* Copyright © 2021 Caden Miller, All Rights Reserved. */

#include "Core/FMemory.h"
#include "../../FGraphicsContext_Impl.h"
#include "FGraphicsContextVulkan.h"
#include "FFramebufferVulkan.h"
#include "FRenderPassVulkan.h"

FFramebuffer* FFramebufferVulkanCreate(FGraphicsContext* pContext, const FFramebufferCreateInfo* pInfo)
{
    if (pInfo == NULL || pInfo->pRenderPass == NULL || pInfo->attachmentsCount <= 0 || pInfo->ppAttachments == NULL || pInfo->width <= 0 || pInfo->height <= 0)
    {
        return NULL;
    }

    FGraphicsContextVulkan* pContextVulkan = (FGraphicsContextVulkan*)pContext->pRenderContext;
    FRenderPassVulkan* pRenderPass = (FRenderPassVulkan*)pInfo->pRenderPass;

    FFramebufferVulkan* pFramebuffer = FAllocateZero(1, sizeof(FFramebufferVulkan));
    if (pFramebuffer == NULL)
    {
        return NULL;
    }

    VkImageView* pAttachments = FAllocateZero(pInfo->attachmentsCount, sizeof(VkImageView));
    if (pAttachments == NULL)
    {
        FDeallocate(pFramebuffer);
        return NULL;
    }

    VkFramebufferCreateInfo createInfo = {
        .sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO,
        .pNext = NULL,
        .flags = 0,
        .renderPass = pRenderPass->renderPass,
        .attachmentCount = pInfo->attachmentsCount,
        .pAttachments = pAttachments,
        .width = pInfo->width,
        .height = pInfo->height,
        .layers = 1  
    };

    if (vkCreateFramebuffer(pContextVulkan->device, &createInfo, NULL, &pFramebuffer->framebuffer) != VK_SUCCESS)
    {
        FDeallocate(pFramebuffer);
        FDeallocate(pAttachments);
        return NULL;
    }

    FDeallocate(pAttachments);

    return (FFramebuffer*)pFramebuffer;
}

void FFramebufferVulkanDestroy(FGraphicsContext* pContext, FFramebuffer** ppFramebuffer)
{
    if (pContext == NULL || ppFramebuffer == NULL || *ppFramebuffer == NULL)
    {
        return;
    }

    FGraphicsContextVulkan* pContextVulkan = (FGraphicsContextVulkan*)pContext->pRenderContext;
    FFramebufferVulkan* pFramebufferVulkan = (FFramebufferVulkan*)*ppFramebuffer;
    
    if (pFramebufferVulkan->framebuffer != VK_NULL_HANDLE)
    {
        vkDestroyFramebuffer(pContextVulkan->device, pFramebufferVulkan->framebuffer, NULL);
    }

    FDeallocate(pFramebufferVulkan);
}