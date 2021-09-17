#ifndef __FFRAMEBUFFER_VULKAN_H__
#define __FFRAMEBUFFER_VULKAN_H__

#include <vulkan/vulkan.h>

#include "Graphics/FFramebuffer.h"

typedef struct FFramebufferVulkan
{
    VkFramebuffer framebuffer;
} FFramebufferVulkan;

FFramebuffer* FFramebufferVulkanCreate(const FFramebufferCreateInfo* pInfo);
void FFramebufferVulkanDestroy(FFramebuffer** ppFramebuffer);

#endif