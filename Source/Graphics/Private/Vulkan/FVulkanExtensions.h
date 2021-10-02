/* Copyright © 2021 Caden Miller, All Rights Reserved. */

#ifndef __FVULKAN_VALIDATION_H__
#define __FVULKAN_VALIDATION_H__

#include <vulkan/vulkan.h>

#include "Core/FMacro.h"
#include "Core/FMemory.h"

const char* const* FVulkanValidationLayers(uint32* pCount);
const char* const* FVulkanInstanceExtensions(uint32* pCount);
const char* const* FVulkanDeviceExtensions(VkPhysicalDevice physicalDevice, uint32* pCount);

#endif