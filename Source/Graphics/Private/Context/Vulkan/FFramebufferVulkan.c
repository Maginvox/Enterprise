#include "Core/FMemory.h"
#include "FFramebufferVulkan.h"

FFramebuffer* FFramebufferVulkanCreate(const FFramebufferCreateInfo* pInfo)
{
    if (pInfo == NULL || pInfo->pRenderPass == NULL || pInfo->attachmentsCount <= 0 || pInfo->ppAttachments == NULL || pInfo->width <= 0 || pInfo->height <= 0)
    {
        return NULL;
    }

    VkImageView* pAttachments = FAllocateZero(pInfo->attachmentsCount, sizeof(VkImageView));
    if (pAttachments == NULL)
    {
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

    vkCreateFramebuffer()
}

void FFramebufferVulkanDestroy(FFramebuffer** ppFramebuffer)
{

}