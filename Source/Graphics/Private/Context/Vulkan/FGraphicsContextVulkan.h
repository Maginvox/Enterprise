/* Copyright © 2021 Caden Miller, All Rights Reserved. */

#ifndef __FRENDERCONTEXT_VULKAN_H__
#define __FRENDERCONTEXT_VULKAN_H__

#include <vulkan/vulkan.h> /* We use the Vulkan 1.1 specification with extensions. https://www.khronos.org/registry/vulkan/specs/1.1-extensions/html/vkspec.html#extensions */
#include <vk_mem_alloc.h>

#include <Core/FTypes.h>
#include "Graphics/FGraphicsContext.h"

typedef struct FGraphicsContextVulkan
{
    VkInstance instance;
    VkDebugUtilsMessengerEXT debugMessenger;

    VkPhysicalDevice physicalDevice;
    FInt32 graphicsFamilyIndex, presentFamilyIndex, computeFamilyIndex;
    VkQueue graphicsQueue, presentQueue, computeQueue;
    bool bHasCompute;
    VkDevice device;
    
    VmaAllocator allocator;
} FGraphicsContextVulkan;

FGraphicsContext* FGraphicsContextVulkanCreate();
void FGraphicsContextVulkanDestroy(FGraphicsContext** ppContext);

void FGraphicsContextVulkanGetOptions(FGraphicsContext* pContext, FGraphicsOptions* pOptions);
void FGraphicsContextVulkanApplyOptions(FGraphicsContext* pContext, const FGraphicsOptions* pOption);

#endif