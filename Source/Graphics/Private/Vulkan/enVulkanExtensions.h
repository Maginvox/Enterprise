/* Copyright © 2021 Caden Miller, All Rights Reserved. */

#ifndef __FVULKAN_VALIDATION_H__
#define __FVULKAN_VALIDATION_H__

#include <vulkan/vulkan.h>

const char* const* enVulkanValidationLayers(uint32* pCount);
const char* const* enVulkanInstanceExtensions(uint32* pCount);
const char* const* enVulkanDeviceExtensions(VkPhysicalDevice physicalDevice, uint32* pCount);

#endif