#ifndef __FWINDOW_VULKAN_H__
#define __FWINDOW_VULKAN_H__

#include <vulkan/vulkan.h>

#include "FGraphicsVulkan.h"

VkSurfaceFormatKHR FWindowVulkanSurfaceFormat(FContextVulkan* pWindow);
VkPresentModeKHR FWindowVulkanPresentMode(FContextVulkan* pWindow);

#endif