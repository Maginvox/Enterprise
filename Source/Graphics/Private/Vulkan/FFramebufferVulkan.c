/* Copyright © 2021 Caden Miller, All Rights Reserved. */

#include "Core/FMemory.h"
#include "FGraphicsVulkan.h"
#include "FFramebufferVulkan.h"
#include "FRenderPassVulkan.h"

FFramebuffer* FFramebufferVulkanCreate(const FFramebufferCreateInfo* pInfo)
{
    if (pInfo == NULL || pInfo->pRenderPass == NULL || pInfo->attachmentsCount <= 0 || pInfo->ppAttachments == NULL || pInfo->width <= 0 || pInfo->height <= 0)
    {
        return NULL;
    }

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

    if (vkCreateFramebuffer(graphics_vk.device, &createInfo, NULL, &pFramebuffer->framebuffer) != VK_SUCCESS)
    {
        FDeallocate(pFramebuffer);
        FDeallocate(pAttachments);
        return NULL;
    }

    FDeallocate(pAttachments);

    return (FFramebuffer*)pFramebuffer;
}

void FFramebufferVulkanDestroy(FFramebuffer* pFramebuffer)
{
    if (pFramebuffer == NULL)
    {
        return;
    }

    FFramebufferVulkan* pFramebufferVulkan = (FFramebufferVulkan*)pFramebuffer;
    
    if (pFramebufferVulkan->framebuffer != VK_NULL_HANDLE)
    {
        vkDestroyFramebuffer(graphics_vk.device, pFramebufferVulkan->framebuffer, NULL);
    }

    FDeallocate(pFramebufferVulkan);
}