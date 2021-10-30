#ifndef __ENTERPRISE_WINDOW_VULKAN_H__
#define __ENTERPRISE_WINDOW_VULKAN_H__

#include <vulkan/vulkan.h>

#include "Core/enTypes.h"


#define ENTERPRISE_VULKAN_MAX_FRAMES 2

typedef struct enWindowVulkan
{
    VkSurfaceKHR surface;
    VkSurfaceFormatKHR surfaceFormat;
    uint32 imageCount;
    VkPresentModeKHR presentMode;
    VkSwapchainKHR swapchain;

    VkSemaphore imageAvailableSemaphores[ENTERPRISE_VULKAN_MAX_FRAMES];
    VkSemaphore renderFinishedSemaphores[ENTERPRISE_VULKAN_MAX_FRAMES];
    VkFence inFlightFences[ENTERPRISE_VULKAN_MAX_FRAMES];
    VkFence imagesInFlight[ENTERPRISE_VULKAN_MAX_FRAMES];
    uint32 currentFrame;
} enWindowVulkan;

extern enWindowVulkan window_vk;

bool enVulkanWindowInitialize();
void enVulkanWindowShutdown();

bool enVulkanCreateSwapchain();
void enVulkanDestroySwapchain();

#endif