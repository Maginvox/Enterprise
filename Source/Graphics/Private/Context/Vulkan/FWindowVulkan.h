#ifndef __FWINDOW_VULKAN_H__
#define __FWINDOW_VULKAN_H__

/* Define the vulkan window api for surface creation */

#include <vulkan/vulkan.h>

typedef struct FWindowVulkan
{
    VkSurface surface;
    VkSwapchain swapchain;
} FWindowVulkan;


#endif