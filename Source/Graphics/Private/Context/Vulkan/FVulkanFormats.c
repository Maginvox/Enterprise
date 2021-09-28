#include <vulkan/vulkan.h>

#include "Core/FMemory.h"
#include "Core/FLog.h"
#include "FVulkanFormats.h"


VkSurfaceFormatKHR FWindowVulkanSurfaceFormat(FContextVulkan* pContext)
{
    if (pContext == NULL)
    {
        return (VkSurfaceFormatKHR){VK_FORMAT_UNDEFINED, VK_COLOR_SPACE_SRGB_NONLINEAR_KHR};
    }
    
    FUInt surfaceFormatsCount = 0;
    if (vkGetPhysicalDeviceSurfaceFormatsKHR(graphics_vk.physicalDevice, pContext->surface, &surfaceFormatsCount, NULL) != VK_SUCCESS)
    {
        return (VkSurfaceFormatKHR){VK_FORMAT_UNDEFINED, VK_COLOR_SPACE_SRGB_NONLINEAR_KHR};
    }

    VkSurfaceFormatKHR* pSurfaceFormats = FAllocateZero(surfaceFormatsCount, sizeof(VkSurfaceFormatKHR));
    if (pSurfaceFormats == NULL)
    {
        return (VkSurfaceFormatKHR){VK_FORMAT_UNDEFINED, VK_COLOR_SPACE_SRGB_NONLINEAR_KHR};
    }

    vkGetPhysicalDeviceSurfaceFormatsKHR(graphics_vk.physicalDevice, pContext->surface, &surfaceFormatsCount, pSurfaceFormats);

    for (FUInt i = 0; i < surfaceFormatsCount; i++)
    {
        if (pSurfaceFormats[i].colorSpace == VK_COLOR_SPACE_SRGB_NONLINEAR_KHR || pSurfaceFormats[i].format == VK_FORMAT_R8G8B8A8_SRGB)
        {
            return pSurfaceFormats[i];
        } 
    }

    return pSurfaceFormats[0]; /* If the specific good format is not found we can really use any. */
}

VkPresentModeKHR FWindowVulkanPresentMode(FContextVulkan* pContext)
{
    if (graphics_vk.physicalDevice == VK_NULL_HANDLE || pContext == NULL)
    {
        return VK_PRESENT_MODE_FIFO_KHR;
    }

    FUInt presentModesCount;
    if (vkGetPhysicalDeviceSurfacePresentModesKHR(graphics_vk.physicalDevice, pContext->surface, &presentModesCount, NULL) != VK_SUCCESS)
    {
        return VK_PRESENT_MODE_FIFO_KHR;
    }

    VkPresentModeKHR* pPresentModes = FAllocateZero(presentModesCount, sizeof(VkPresentModeKHR));
    if (pPresentModes == NULL)
    {
        return VK_PRESENT_MODE_FIFO_KHR;
    }

    vkGetPhysicalDeviceSurfacePresentModesKHR(graphics_vk.physicalDevice, pContext->surface, &presentModesCount, pPresentModes);

    for (FUInt32 i = 0; i < presentModesCount; i++)
    {
        if (pPresentModes[i] == VK_PRESENT_MODE_MAILBOX_KHR)
        {
            return VK_PRESENT_MODE_MAILBOX_KHR;
        }
    }

    return VK_PRESENT_MODE_FIFO_KHR;
}