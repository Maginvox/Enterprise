/* Copyright © 2021 Caden Miller, All Rights Reserved. */

#ifndef __FVULKAN_VALIDATION_H__
#define __FVULKAN_VALIDATION_H__

#include <vulkan/vulkan.h>

const char* const* FVulkanValidationLayers(FInt32* pCount);
const char* const* FVulkanInstanceExtensions(FInt32* pCount);
const char* const* FVulkanDeviceExtensions(VkPhysicalDevice physicalDevice, FInt32* pCount);

#endif