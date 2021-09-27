#ifdef ENTERPRISE_WINDOWS
    #include <Windows.h>
    #define VK_USE_PLATFORM_WIN32_KHR
#elif ENTERPRISE_MACOS
    #error Need to implement.
#elif ENTERPRISE_ANDROID
    #define VK_USE_PLATFORM_ANDROID_KHR
#elif ENTERPRISE_LINUX
    #include <X11/Xlib.h>
    #define VK_USE_PLATFORM_XLIB_KHR
#endif
#include <vulkan/vulkan.h>

#include "Core/FMemory.h"
#include "Core/FLog.h"
#include "FWindowVulkan.h"

FWindowVulkan* FWindowVulkanCreate(FGraphicsContextVulkan* pContext, FWindow* pWindow)
{
    if (pContext == NULL || pWindow == NULL)
    {
        return NULL;
    }

    /* Create the vulkan window */
    FWindowVulkan* pWindowVulkan = FAllocateZero(1, sizeof(FWindowVulkan));
    if (pWindowVulkan == NULL)
    {
        return NULL;
    }

    /* Create the platform surface */
#ifdef ENTERPRISE_WINDOWS
    
    VkWin32SurfaceCreateInfoKHR surfaceCreateInfo = {
        .sType = VK_STRUCTURE_TYPE_WIN32_SURFACE_CREATE_INFO_KHR,
        .pNext = NULL,
        .flags = 0,
        .hinstance = GetModuleHandle(NULL),
        .hwnd = FWindowGetSystemHandle(pWindow)
    };

    if (vkCreateWin32SurfaceKHR(pContext->instance, &surfaceCreateInfo, NULL, &pWindowVulkan->surface) != VK_SUCCESS)
    {
        FWindowVulkanDestroy(pContext, pWindowVulkan);
        FLogError("Could not create the vulkan surface!");
        return NULL; 
    }

#elif ENTERPRISE_MACOS
    #error Need to implement.
#elif ENTERPRISE_ANDROID
    #error Need to implement.
#elif ENTERPRISE_LINUX
    
    VkXlibSurfaceCreateInfoKHR surfaceCreateInfo = {
        .sType = VK_STRUCTURE_TYPE_XLIB_SURFACE_CREATE_INFO_KHR,
        .pNext = NULL,
        .flags = 0,
        .dpy = FWindowSystemHandle(),
        .window = (Window)FWindowGetSystemHandle(pWindow)
    };

    if (vkCreateXlibSurfaceKHR(pContext->instance, &surfaceCreateInfo, NULL, &pWindowVulkan->surface) != VK_SUCCESS)
    {
        FWindowVulkanDestroy(pContext, pWindow, pWindowVulkan);
        FLogError("Could not create the vulkan surface!");
        return NULL;
    }
#endif

    /* Create the swapchain */
    if (pContext->device == VK_NULL_HANDLE) /* In the device is not yet created we can't create the swapchain, the context will do it for us. */
    {
        if (!FWindowVulkanCreateSwapchain(pContext, pWindow, pWindowVulkan))
        {
            FWindowVulkanDestroy(pContext, pWindow, pWindowVulkan);
            FLogError("Could not create the vulkan surface!");
            return NULL;
        }
    }

    return pWindowVulkan;
}

void FWindowVulkanDestroy(FGraphicsContextVulkan* pContext, FWindow* pWindow, FWindowVulkan* pWindowVulkan)
{
    FWindowVulkanDestroySwapchain(pContext, pWindowVulkan);

    if (pWindowVulkan->surface != VK_NULL_HANDLE)
    {
        vkDestroySurfaceKHR(pContext->instance, pWindowVulkan->surface, NULL);
    }
}

bool FWindowVulkanCreateSwapchain(FGraphicsContextVulkan* pContext, FWindow* pWindow, FWindowVulkan* pWindowVulkan)
{
    /* Get the surface information */

    VkSurfaceCapabilitiesKHR surfaceCapabilities = {0};
    vkGetPhysicalDeviceSurfaceCapabilitiesKHR(pContext->physicalDevice, pWindowVulkan->surface, &surfaceCapabilities);

    pWindowVulkan->imageCount = surfaceCapabilities.minImageCount + 1 <= surfaceCapabilities.maxImageCount ? surfaceCapabilities.minImageCount + 1 : surfaceCapabilities.minImageCount;

    pWindowVulkan->surfaceFormat = FWindowVulkanSurfaceFormat(pContext, pWindowVulkan);
    
    FUInt32 width = 0, height = 0;
    FWindowGetSize(pWindow, &width, &height);

    pWindowVulkan->presentMode = FWindowVulkanPresentMode(pContext, pWindowVulkan);

    VkSwapchainCreateInfoKHR swapchainCreateInfo = {
        .sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR,
        .pNext = NULL,
        .flags = 0,
        .surface = pWindowVulkan->surface,
        .minImageCount = pWindowVulkan->imageCount,
        .imageFormat = pWindowVulkan->surfaceFormat.format,
        .imageColorSpace = pWindowVulkan->surfaceFormat.colorSpace,
        .imageExtent = (VkExtent2D){width, height},
        .imageArrayLayers = 1,
        .imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT | VK_IMAGE_USAGE_TRANSFER_DST_BIT,
        .imageSharingMode = VK_SHARING_MODE_EXCLUSIVE,
        .queueFamilyIndexCount = 0,
        .pQueueFamilyIndices = NULL,
        .preTransform = surfaceCapabilities.currentTransform,
        .compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR,
        .presentMode = pWindowVulkan->presentMode,
        .clipped = VK_FALSE,
        .oldSwapchain = VK_NULL_HANDLE
    };

    if (vkCreateSwapchainKHR(pContext->device, &swapchainCreateInfo, NULL, &pWindowVulkan->swapchain) != VK_SUCCESS)
    {
        return false;
    }

    return true;
}

void FWindowVulkanDestroySwapchain(FGraphicsContextVulkan* pContext, FWindowVulkan* pWindowVulkan)
{
    if (pContext == NULL || pWindowVulkan == NULL)
    {
        return;
    }

    if (pWindowVulkan->swapchain != VK_NULL_HANDLE)
    {
        vkDestroySwapchainKHR(pContext->device, pWindowVulkan->swapchain, NULL);
    }
}

VkSurfaceFormatKHR FWindowVulkanSurfaceFormat(FGraphicsContextVulkan* pContext, FWindowVulkan* pWindow)
{
    if (pContext == NULL || pWindow == NULL)
    {
        return (VkSurfaceFormatKHR){VK_FORMAT_UNDEFINED, VK_COLOR_SPACE_SRGB_NONLINEAR_KHR};
    }
    
    FUInt surfaceFormatsCount = 0;
    if (vkGetPhysicalDeviceSurfaceFormatsKHR(pContext->physicalDevice, pWindow->surface, &surfaceFormatsCount, NULL) != VK_SUCCESS)
    {
        return (VkSurfaceFormatKHR){VK_FORMAT_UNDEFINED, VK_COLOR_SPACE_SRGB_NONLINEAR_KHR};
    }

    VkSurfaceFormatKHR* pSurfaceFormats = FAllocateZero(surfaceFormatsCount, sizeof(VkSurfaceFormatKHR));
    if (pSurfaceFormats == NULL)
    {
        return (VkSurfaceFormatKHR){VK_FORMAT_UNDEFINED, VK_COLOR_SPACE_SRGB_NONLINEAR_KHR};
    }

    vkGetPhysicalDeviceSurfaceFormatsKHR(pContext->physicalDevice, pWindow->surface, &surfaceFormatsCount, pSurfaceFormats);

    for (FUInt i = 0; i < surfaceFormatsCount; i++)
    {
        if (pSurfaceFormats[i].colorSpace == VK_COLOR_SPACE_SRGB_NONLINEAR_KHR || pSurfaceFormats[i].format == VK_FORMAT_R8G8B8A8_SRGB)
        {
            return pSurfaceFormats[i];
        } 
    }

    return pSurfaceFormats[0]; /* If the specific good format is not found we can really use any. */
}

VkPresentModeKHR FWindowVulkanPresentMode(FGraphicsContextVulkan* pContext, FWindowVulkan* pWindowVulkan)
{
    if (pContext == NULL || pWindowVulkan == NULL)
    {
        return VK_PRESENT_MODE_FIFO_KHR;
    }

    FUInt presentModesCount;
    if (vkGetPhysicalDeviceSurfacePresentModesKHR(pContext->physicalDevice, pWindowVulkan->surface, &presentModesCount, NULL) != VK_SUCCESS)
    {
        return VK_PRESENT_MODE_FIFO_KHR;
    }

    VkPresentModeKHR* pPresentModes = FAllocateZero(presentModesCount, sizeof(VkPresentModeKHR));
    if (pPresentModes == NULL)
    {
        return VK_PRESENT_MODE_FIFO_KHR;
    }

    vkGetPhysicalDeviceSurfacePresentModesKHR(pContext->physicalDevice, pWindowVulkan->surface, &presentModesCount, pPresentModes);

    for (FUInt32 i = 0; i < presentModesCount; i++)
    {
        if (pPresentModes[i] == VK_PRESENT_MODE_MAILBOX_KHR)
        {
            return VK_PRESENT_MODE_MAILBOX_KHR;
        }
    }

    return VK_PRESENT_MODE_FIFO_KHR;
}