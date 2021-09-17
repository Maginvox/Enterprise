#include "Core/FMemory.h"
#include "../../FGraphicsContext_Impl.h"
#include "FGraphicsContextVulkan.h"
#include "FFramebufferVulkan.h"

FFramebuffer* FFramebufferVulkanCreate(FGraphicsContext* pContext, const FFramebufferCreateInfo* pInfo)
{
    if (pInfo == NULL || pInfo->pRenderPass == NULL || pInfo->attachmentsCount <= 0 || pInfo->ppAttachments == NULL || pInfo->width <= 0 || pInfo->height <= 0)
    {
        return NULL;
    }

    FGraphicsContextVulkan* pContextVulkan = (FGraphicsContextVulkan*)pContext->pRenderContext;

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

void FFramebufferVulkanDestroy(FFramebuffer** ppFramebuffer)
{
    if (ppFramebuffer == NULL || *ppFramebuffer == NULL)
    {
        return;
    }

    vkDestroyFramebuffer()
}