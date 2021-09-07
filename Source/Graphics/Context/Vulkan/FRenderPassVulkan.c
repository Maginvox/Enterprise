#include "Core/FMemory.h"
#include "Graphics/Context/Vulkan/FVulkanUtils.h"
#include "Graphics/Context/Vulkan/FRenderPassVulkan.h"

FRenderPass* FRenderPassCreateVulkan(FRenderContext* pContext, const FRenderPassCreateInfo* pInfo)
{
    if (pContext == NULL || pInfo == NULL)
    {
        return NULL;
    }

    
    VkAttachmentDescription* pAttachmentDescriptions = FAllocateZero(pInfo->colorAttachmentsCount, sizeof(VkAttachmentDescription));
    if (pAttachmentDescriptions == NULL)
    {
        return NULL;
    }

    for (FInt32 i = 0; i < pInfo->colorAttachmentsCount; i++)
    {
        FRenderPassAttachment* pAttachment = &pInfo->pColorAttachments[i];

        pAttachmentDescriptions[i].flags = 0;
        pAttachmentDescriptions[i].format = FImageFormatToVkFormat(pAttachment->format);
        pAttachmentDescriptions[i].samples = VK_SAMPLE_COUNT_1_BIT;
        pAttachmentDescriptions[i].loadOp = FRenderPassAttachmentLoadOpToVkAttachmentLoadOp(pAttachment->loadOp);
        pAttachmentDescriptions[i].storeOp = FRenderPassAttachmentStoreOpToVkAttachmentStoreOp(pAttachment->storeOp);
        pAttachmentDescriptions[i].stencilLoadOp = FRenderPassAttachmentLoadOpToVkAttachmentLoadOp(pAttachment->stencilLoadOp);
        pAttachmentDescriptions[i].stencilStoreOp = FRenderPassAttachmentStoreOpToVkAttachmentStoreOp(pAttachment->stencilStoreOp);
        pAttachmentDescriptions[i].initialLayout = 
    }

    VkRenderPassCreateInfo createInfo = {
        .sType = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO,
        .pNext = NULL,
        .flags = 0,
        .attachmentCount = 
    };

}