/* Copyright © 2021 Caden Miller, All Rights Reserved. */

#ifndef __FGRAPHICS_VULKAN_H__
#define __FGRAPHICS_VULKAN_H__

#include <vulkan/vulkan.h> /* We use the Vulkan 1.1 specification with extensions. https://www.khronos.org/registry/vulkan/specs/1.1-extensions/html/vkspec.html#extensions */
#include <vk_mem_alloc.h>

#include <Core/FTypes.h>
#include "Graphics/FGraphics.h"

typedef struct FGraphicsVulkan
{
    VkInstance instance;
    PFN_vkCreateDebugUtilsMessengerEXT pCreateDebugMessenger;
    PFN_vkDestroyDebugUtilsMessengerEXT pDestroyDebugMessenger;
    VkDebugUtilsMessengerEXT debugMessenger;

    VkPhysicalDevice physicalDevice;
    FInt32 graphicsFamilyIndex, presentFamilyIndex, computeFamilyIndex;
    VkQueue graphicsQueue, presentQueue, computeQueue;
    bool bHasCompute;
    VkDevice device;
    
    VmaAllocator allocator;
} FGraphicsVulkan;

typedef struct FContextVulkan
{
    VkSurfaceKHR surface;
    VkSurfaceFormatKHR surfaceFormat;
    FUInt32 imageCount;
    VkPresentModeKHR presentMode;
    VkSwapchainKHR swapchain;
} FContextVulkan;

bool FContextVulkanCreateSwapchain(FWindow window, FContextVulkan* pContext);
void FContextVulkanDestroySwapchain(FWindow window, FContextVulkan* pContext);

extern FGraphicsVulkan graphics_vk;

#endif