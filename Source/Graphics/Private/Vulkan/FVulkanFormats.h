#ifndef __FWINDOW_VULKAN_H__
#define __FWINDOW_VULKAN_H__

#include <vulkan/vulkan.h>

#include "FWindowVulkan.h"

VkSurfaceFormatKHR FWindowVulkanSurfaceFormat();
VkPresentModeKHR FWindowVulkanPresentMode();

#endif