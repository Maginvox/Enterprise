#ifndef __FWINDOW_VULKAN_H__
#define __FWINDOW_VULKAN_H__

#include <vulkan/vulkan.h>

#include "enWindowVulkan.h"

VkSurfaceFormatKHR enWindowVulkanSurfaceFormat();
VkPresentModeKHR enWindowVulkanPresentMode();

#endif