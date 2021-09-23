#ifndef __FWINDOW_VULKAN_H__
#define __FWINDOW_VULKAN_H__

#include <vulkan/vulkan.h>

#include "Window/FWindow.h"

typedef struct FWindowVulkan
{
    VkSurface surface;
    VkSwapchain swapchain;
} FWindowVulkan;

FWindowVulkan* FWindowVulkanCreate(FWindow* pWindow);
void FWindowVulkanDestroy(FWindowVulkan* pWindow);

#endif