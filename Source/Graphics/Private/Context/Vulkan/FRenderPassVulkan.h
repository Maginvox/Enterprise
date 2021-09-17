#ifndef __FRENDERPASS_VULKAN_H__
#define __FRENDERPASS_VULKAN_H__

#include <vulkan/vulkan.h>

#include "Graphics/FRenderPass.h"

typedef struct FRenderPassVulkan
{
    VkRenderPass renderPass;
} FRenderPassVulkan;

FRenderPass* FRenderPassVulkanCreate(FGraphicsContext* pContext, const FRenderPassCreateInfo* pInfo);
void FRenderPassVulkanDestroy(FGraphicsContext* pContext, FRenderPass** ppRenderPass);

#endif