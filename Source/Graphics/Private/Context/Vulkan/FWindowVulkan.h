#ifndef __FWINDOW_VULKAN_H__
#define __FWINDOW_VULKAN_H__

#include <vulkan/vulkan.h>

#include "Window/FWindow.h"
#include "FGraphicsContextVulkan.h"

typedef struct FWindowVulkan
{
    VkSurfaceKHR surface;
    VkSwapchainKHR swapchain;
} FWindowVulkan;

FWindowVulkan* FWindowVulkanCreate(FGraphicsContextVulkan* pContext, FWindow* pWindow); /* Will also create the swapchain if pContext->device is not VK_NULL_HANDLE */
void FWindowVulkanDestroy(FGraphicsContextVulkan* pContext, FWindowVulkan* pWindow);

void FWindowVulkanCreateSwapchain(FGraphicsContextVulkan* pContext, FWindowVulkan* pWindow);
void FWindowVulkanDestroySwapchain(FGraphicsContextVulkan* pContext, FWindowVulkan* pWindow);

#endif