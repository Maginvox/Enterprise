#ifndef __ENTERPRISE_WINDOW_VULKAN_H__
#define __ENTERPRISE_WINDOW_VULKAN_H__

#include <vulkan/vulkan.h>

#include "Core/enTypes.h"

typedef struct enWindowVulkan
{
    VkSurfaceKHR surface;
    VkSurfaceFormatKHR surfaceFormat;
    uint32 imageCount;
    VkPresentModeKHR presentMode;
    VkSwapchainKHR swapchain;
} enWindowVulkan;

extern enWindowVulkan window_vk;

bool enWindowVulkanInitialize();
void enWindowVulkanShutdown();

bool enWindowVulkanCreateSwapchain();
void enWindowVulkanDestroySwapchain();

#endif