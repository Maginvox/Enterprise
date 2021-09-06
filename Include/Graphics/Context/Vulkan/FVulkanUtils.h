#ifndef __FVULKAN_UTILS_H__
#define __FVULKAN_UTILS_H__

#include <vulkan/vulkan.h>
#include "Graphics/Vulkan/FRenderDeviceVulkan.h"

static inline VkFormat ETextureFormatToVkFormat(ETextureFormat format)
{
    switch (format)
    {
        case E_TEXTURE_FORMAT_RGBA8:
            return VK_FORMAT_R8G8B8A8_SRGB;
            break;
        case E_TEXTURE_FORMAT_D24S8:
            return VK_FORMAT_D24_UNORM_S8_UINT;
            break;
        default:
            return VK_FORMAT_UNDEFINED;
    }
}

#endif