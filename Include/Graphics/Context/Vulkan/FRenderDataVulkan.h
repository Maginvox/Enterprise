#ifndef __FRENDERCONTEXT_VULKAN_H__
#define __FRENDERCONTEXT_VULKAN_H__

#include <vulkan/vulkan.h>

typedef struct FRenderDataVulkan
{
    VkInstance instance;
} FRenderDataVulkan;

FRenderDataVulkan* FRenderDataVulkanCreate();
void FRenderDataVulkanDestroy(FRenderDataVulkan** ppData);

#endif