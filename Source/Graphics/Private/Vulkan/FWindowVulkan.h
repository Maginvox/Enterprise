#ifndef __FCONTEXT_VULKAN_H__
#define __FCONTEXT_VULKAN_H__

#include <vulkan/vulkan.h>

#include "Core/FTypes.h"

typedef struct FWindowVulkan
{
    VkSurfaceKHR surface;
    VkSurfaceFormatKHR surfaceFormat;
    FUInt32 imageCount;
    VkPresentModeKHR presentMode;
    VkSwapchainKHR swapchain;
} FWindowVulkan;

extern FWindowVulkan window_vk;

bool FWindowVulkanInitialize();
void FWindowVulkanShutdown();

bool FWindowVulkanCreateSwapchain();
void FWindowVulkanDestroySwapchain();

#endif