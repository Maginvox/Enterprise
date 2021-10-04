#include <vulkan/vulkan.h>

#include "Core/FMemory.h"
#include "Core/FLog.h"

#include "FVulkanFormats.h"
#include "FGraphicsVulkan.h"


VkSurfaceFormatKHR FWindowVulkanSurfaceFormat()
{

    uint32_t surfaceFormatsCount = 0;
    if (vkGetPhysicalDeviceSurfaceFormatsKHR(graphics_vk.physicalDevice, window_vk.surface, &surfaceFormatsCount, NULL) != VK_SUCCESS)
    {
        return (VkSurfaceFormatKHR){VK_FORMAT_UNDEFINED, VK_COLOR_SPACE_SRGB_NONLINEAR_KHR};
    }

    VkSurfaceFormatKHR* pSurfaceFormats = enAllocateZero(surfaceFormatsCount, sizeof(VkSurfaceFormatKHR));
    if (pSurfaceFormats == NULL)
    {
        return (VkSurfaceFormatKHR){VK_FORMAT_UNDEFINED, VK_COLOR_SPACE_SRGB_NONLINEAR_KHR};
    }

    vkGetPhysicalDeviceSurfaceFormatsKHR(graphics_vk.physicalDevice, window_vk.surface, &surfaceFormatsCount, pSurfaceFormats);

    for (uint32 i = 0; i < surfaceFormatsCount; i++)
    {
        if (pSurfaceFormats[i].colorSpace == VK_COLOR_SPACE_SRGB_NONLINEAR_KHR || pSurfaceFormats[i].format == VK_FORMAT_R8G8B8A8_SRGB)
        {
            return pSurfaceFormats[i];
        } 
    }

    return pSurfaceFormats[0]; /* If the specific good format is not found we can really use any. */
}

VkPresentModeKHR FWindowVulkanPresentMode()
{

    uint32 presentModesCount;
    if (vkGetPhysicalDeviceSurfacePresentModesKHR(graphics_vk.physicalDevice, window_vk.surface, &presentModesCount, NULL) != VK_SUCCESS)
    {
        return VK_PRESENT_MODE_FIFO_KHR;
    }

    VkPresentModeKHR* pPresentModes = enAllocateZero(presentModesCount, sizeof(VkPresentModeKHR));
    if (pPresentModes == NULL)
    {
        return VK_PRESENT_MODE_FIFO_KHR;
    }

    vkGetPhysicalDeviceSurfacePresentModesKHR(graphics_vk.physicalDevice, window_vk.surface, &presentModesCount, pPresentModes);

    for (uint32 i = 0; i < presentModesCount; i++)
    {
        if (pPresentModes[i] == VK_PRESENT_MODE_MAILBOX_KHR)
        {
            return VK_PRESENT_MODE_MAILBOX_KHR;
        }
    }

    return VK_PRESENT_MODE_FIFO_KHR;
}