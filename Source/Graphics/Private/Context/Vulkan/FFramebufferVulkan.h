/* Copyright © 2021 Caden Miller, All Rights Reserved. */

#ifndef __FFRAMEBUFFER_VULKAN_H__
#define __FFRAMEBUFFER_VULKAN_H__

#include <vulkan/vulkan.h>

#include "Graphics/FFramebuffer.h"

typedef struct FFramebufferVulkan
{
    VkFramebuffer framebuffer;
} FFramebufferVulkan;

#endif