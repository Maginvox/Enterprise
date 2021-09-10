#ifndef __FRENDERPASS_VULKAN_H__
#define __FRENDERPASS_VULKAN_H__

#include <vulkan/vulkan.h>
#include <vk_mem_alloc.h>

#include "Graphics/FRenderPass.h"

typedef struct FRenderPassVulkan
{
    VkRenderPass renderPass;
} FRenderPassVulkan;

FRenderPass* FRenderPassCreateVulkan(FRenderContext* pContext, const FRenderPassCreateInfo* pInfo);
void FRenderPassDestroy(FRenderContext* pContext, FRenderPass** ppRenderPass);

#endif