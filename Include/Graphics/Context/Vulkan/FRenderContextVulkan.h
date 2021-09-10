#ifndef __FRENDERCONTEXT_VULKAN_H__
#define __FRENDERCONTEXT_VULKAN_H__

#include <FTypes.h>

#include <vulkan/vulkan.h>
#include <vk_mem_alloc.h>

#include "Graphics/FRenderContext.h"

typedef struct FRenderContextVulkan
{
    VkInstance instance;
    VkDebugUtilsMessengerEXT debugMessenger;

    VkPhysicalDevice physicalDevice;
    FInt32 graphicsFamilyIndex, presentFamilyIndex, computeFamilyIndex;
    VkQueue graphicsQueue, presentQueue, computeQueue;
    bool bHasCompute;
    VkDevice device;
    
    VmaAllocator allocator;



} FRenderContextVulkan;

FRenderContext* FRenderContextVulkan_Create(const FRenderContextCreateInfo* pInfo);
void FRenderContextVulkan_Destroy(FRenderContext** ppContext);

#endif