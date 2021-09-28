/* Copyright © 2021 Caden Miller, All Rights Reserved. */

#ifndef __FRENDERPASS_VULKAN_H__
#define __FRENDERPASS_VULKAN_H__

#include <vulkan/vulkan.h>

#include "Graphics/FRenderPass.h"

typedef struct FRenderPassVulkan
{
    VkRenderPass renderPass;
} FRenderPassVulkan;

#endif