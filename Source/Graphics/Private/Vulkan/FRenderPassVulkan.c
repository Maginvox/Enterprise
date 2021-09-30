/* Copyright © 2021 Caden Miller, All Rights Reserved. */

#include "Core/FMemory.h"
#include "FVulkanUtils.h"
#include "FGraphicsVulkan.h"
#include "FRenderPassVulkan.h"

FRenderPass* FRenderPassVulkanCreate(const FRenderPassCreateInfo* pInfo)
{
    if (pInfo == NULL)
    {
        return NULL;
    }

    /* Count the attachment and allocate the descriptions and reference */
    FInt32 attachmentDescriptionCount = pInfo->colorAttachmentsCount + (pInfo->pDepthStencilAttachment != NULL ? 1 : 0);    
    
    VkAttachmentDescription* pAttachmentDescriptions = FAllocateZero(attachmentDescriptionCount, sizeof(VkAttachmentDescription));
    if (pAttachmentDescriptions == NULL)
    {
        return NULL;
    }

    VkAttachmentReference* pAttachmentReferences = FAllocateZero(attachmentDescriptionCount, sizeof(VkAttachmentReference));
    if (pAttachmentReferences == NULL)
    {
        FDeallocate(pAttachmentDescriptions);
        return NULL;
    }

    /* Insert the user data into the color attachments */
    FInt32 i = 0;
    for (; i < pInfo->colorAttachmentsCount; i++)
    {
        const FRenderPassAttachment* pAttachment = &pInfo->pColorAttachments[i];

        pAttachmentDescriptions[i].flags = 0;
        pAttachmentDescriptions[i].format = FImageFormatToVkFormat(pAttachment->format);
        pAttachmentDescriptions[i].samples = VK_SAMPLE_COUNT_1_BIT;
        pAttachmentDescriptions[i].loadOp = FRenderPassAttachmentLoadOpToVkAttachmentLoadOp(pAttachment->loadOp);
        pAttachmentDescriptions[i].storeOp = FRenderPassAttachmentStoreOpToVkAttachmentStoreOp(pAttachment->storeOp);
        pAttachmentDescriptions[i].stencilLoadOp = FRenderPassAttachmentLoadOpToVkAttachmentLoadOp(pAttachment->stencilLoadOp);
        pAttachmentDescriptions[i].stencilStoreOp = FRenderPassAttachmentStoreOpToVkAttachmentStoreOp(pAttachment->stencilStoreOp);
        pAttachmentDescriptions[i].initialLayout = FImageLayoutToVkImageLayout(pAttachment->initialLayout);
        pAttachmentDescriptions[i].finalLayout = FImageLayoutToVkImageLayout(pAttachment->finalLayout);

        pAttachmentReferences[i].attachment = i;
        pAttachmentReferences[i].layout = FImageLayoutToVkImageLayout(pInfo->pColorAttachments[i].finalLayout);
    }
    
    /* If were using a depth/stencil attachment description */
    if (pInfo->pDepthStencilAttachment != NULL)
    {
        const FRenderPassAttachment* pAttachment = pInfo->pDepthStencilAttachment;

        pAttachmentDescriptions[i].flags = 0;
        pAttachmentDescriptions[i].format = FImageFormatToVkFormat(pAttachment->format);
        pAttachmentDescriptions[i].samples = VK_SAMPLE_COUNT_1_BIT;
        pAttachmentDescriptions[i].loadOp = FRenderPassAttachmentLoadOpToVkAttachmentLoadOp(pAttachment->loadOp);
        pAttachmentDescriptions[i].storeOp = FRenderPassAttachmentStoreOpToVkAttachmentStoreOp(pAttachment->storeOp);
        pAttachmentDescriptions[i].stencilLoadOp = FRenderPassAttachmentLoadOpToVkAttachmentLoadOp(pAttachment->stencilLoadOp);
        pAttachmentDescriptions[i].stencilStoreOp = FRenderPassAttachmentStoreOpToVkAttachmentStoreOp(pAttachment->stencilStoreOp);
        pAttachmentDescriptions[i].initialLayout = FImageLayoutToVkImageLayout(pAttachment->initialLayout);
        pAttachmentDescriptions[i].finalLayout = FImageLayoutToVkImageLayout(pAttachment->finalLayout);
        
        pAttachmentReferences[i].attachment = i;
        pAttachmentReferences[i].layout = FImageLayoutToVkImageLayout(pAttachment->finalLayout);
        i++;
    }

    /* Add the subpass for the attachments */
    VkSubpassDescription subpassDescription = {
        .flags = 0,
        .pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS,
        .inputAttachmentCount = 0,
        .pInputAttachments = NULL,
        .colorAttachmentCount = pInfo->colorAttachmentsCount,
        .pColorAttachments = pAttachmentReferences,
        .pResolveAttachments = NULL,
        .pDepthStencilAttachment = (pInfo->pDepthStencilAttachment != NULL ? (&pAttachmentReferences[pInfo->colorAttachmentsCount]) : NULL),
        .preserveAttachmentCount = 0,
        .pPreserveAttachments = NULL
    };

    VkPipelineStageFlags sourceStageMask =
                        (pInfo->colorAttachmentsCount != 0 ? VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT : 0) |
                        (pInfo->pDepthStencilAttachment != NULL ? VK_PIPELINE_STAGE_EARLY_FRAGMENT_TESTS_BIT : 0);

    VkAccessFlags destinationAccessMask =
                        (pInfo->colorAttachmentsCount != 0 ? VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT : 0) |
                        (pInfo->pDepthStencilAttachment != NULL ? VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_WRITE_BIT : 0);

    VkSubpassDependency subpassDependency = {
        .srcSubpass = VK_SUBPASS_EXTERNAL,
        .dstSubpass = 0,
        .srcStageMask = sourceStageMask,
        .dstAccessMask = sourceStageMask,
        .srcAccessMask = 0,
        .dstAccessMask = destinationAccessMask
    };

    /* Create the render pass */
    VkRenderPassCreateInfo createInfo = {
        .sType = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO,
        .pNext = NULL,
        .flags = 0,
        .attachmentCount = i,
        .pAttachments = pAttachmentDescriptions,
        .subpassCount = 1,
        .pSubpasses = &subpassDescription,
        .dependencyCount = 1,
        .pDependencies = &subpassDependency
    };

    /* Allocate the render pass */
    FRenderPassVulkan* pRenderPass = FAllocateZero(1, sizeof(FRenderPassVulkan));
    if (pRenderPass == NULL)
    {
        FDeallocate(pAttachmentDescriptions);
        FDeallocate(pAttachmentReferences);
        return NULL;
    }

    if (vkCreateRenderPass(graphics_vk.device, &createInfo, NULL, &pRenderPass->renderPass) != VK_SUCCESS)
    {
        FDeallocate(pAttachmentDescriptions);
        FDeallocate(pAttachmentReferences);
        return NULL;
    }

    FDeallocate(pAttachmentDescriptions);
    FDeallocate(pAttachmentReferences);

    return (FRenderPass*)pRenderPass;
}

void FRenderPassDestroy(FRenderPass* pRenderPass)
{
    if (pRenderPass == NULL)
    {
        return;
    }

    FRenderPassVulkan* pRenderPassVulkan = (FRenderPassVulkan*)pRenderPass;
    
    if (pRenderPassVulkan->renderPass != VK_NULL_HANDLE)
    {
        vkDestroyRenderPass(graphics_vk.device, pRenderPassVulkan->renderPass, NULL);
    }

    FDeallocate(pRenderPassVulkan);
}