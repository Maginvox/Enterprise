#ifdef ENTERPRISE_WINDOWS
    #include <Windows.h>
    #define VK_USE_PLATFORM_WIN32_KHR
#elif ENTERPRISE_MACOS
    #error Need to implement.
#elif ENTERPRISE_ANDROID
    #define VK_USE_PLATFORM_ANDROID_KHR
#elif ENTERPRISE_LINUX
    #include <X11/Xlib.h>
    #define VK_USE_PLATFORM_XCB_KHR
#endif
#include <vulkan/vulkan.h>

#include "Core/FLog.h"

#include "FVulkanFormats.h"
#include "FWindowVulkan.h"
#include "FGraphicsVulkan.h"

FWindowVulkan window_vk;

bool FWindowVulkanInitialize()
{

    /* Create the platform surface */
#ifdef ENTERPRISE_WINDOWS
    
    VkWin32SurfaceCreateInfoKHR surfaceCreateInfo = {
        .sType = VK_STRUCTURE_TYPE_WIN32_SURFACE_CREATE_INFO_KHR,
        .pNext = NULL,
        .flags = 0,
        .hinstance = GetModuleHandle(NULL),
        .hwnd = FWindowGetHandle()
    };

    if (vkCreateWin32SurfaceKHR(graphics_vk.instance, &surfaceCreateInfo, NULL, &window_vk.surface) != VK_SUCCESS)
    {
        enLogError("Could not create the vulkan surface!");
        return false; 
    }

#elif ENTERPRISE_MACOS
    #error Need to implement.
#elif ENTERPRISE_ANDROID
    #error Need to implement.
#elif ENTERPRISE_LINUX
    
    VkXcbSurfaceCreateInfoKHR surfaceCreateInfo = {
        .sType = VK_STRUCTURE_TYPE_XLIB_SURFACE_CREATE_INFO_KHR,
        .pNext = NULL,
        .flags = 0,
        .connection = FWindowGetConnection(),
        .window = FWindowGetHandle()
    };

    if (vkCreateXcbSurfaceKHR(graphics_vk.instance, &surfaceCreateInfo, NULL, &window_vk.surface) != VK_SUCCESS)
    {
        enLogError("Could not create the vulkan surface!");
        return false;
    }
#endif

    /* Create the swapchain */
    if (graphics_vk.device != VK_NULL_HANDLE) /* In the device is not yet created we can't create the swapchain, the context will do it for us. */
    {
        if (!FWindowVulkanCreateSwapchain())
        {
            FWindowVulkanShutdown();
            enLogError("Could not create the vulkan surface!");
            return false;
        }
    }

    return true;
}

void FWindowVulkanShutdown()
{
    FWindowVulkanDestroySwapchain();

    if (window_vk.surface != VK_NULL_HANDLE)
    {
        vkDestroySurfaceKHR(graphics_vk.instance, window_vk.surface, NULL);
    }
}

bool FWindowVulkanCreateSwapchain()
{

    /* Get the surface information */
    VkBool32 surfaceSupported = false;
    vkGetPhysicalDeviceSurfaceSupportKHR(graphics_vk.physicalDevice, graphics_vk.presentFamilyIndex, window_vk.surface, &surfaceSupported);

    if (!surfaceSupported)
    {
        return false;
    }

    VkSurfaceCapabilitiesKHR surfaceCapabilities = {0};
    vkGetPhysicalDeviceSurfaceCapabilitiesKHR(graphics_vk.physicalDevice, window_vk.surface, &surfaceCapabilities);

    window_vk.imageCount = surfaceCapabilities.minImageCount + 1 <= surfaceCapabilities.maxImageCount ? surfaceCapabilities.minImageCount + 1 : surfaceCapabilities.minImageCount;
    window_vk.surfaceFormat = FWindowVulkanSurfaceFormat();
    
    uint32 width = 0, height = 0;
    FWindowGetSize(&width, &height);

    window_vk.presentMode = FWindowVulkanPresentMode();

    VkSwapchainCreateInfoKHR swapchainCreateInfo = {
        .sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR,
        .pNext = NULL,
        .flags = 0,
        .surface = window_vk.surface,
        .minImageCount = window_vk.imageCount,
        .imageFormat = window_vk.surfaceFormat.format,
        .imageColorSpace = window_vk.surfaceFormat.colorSpace,
        .imageExtent = (VkExtent2D){width, height},
        .imageArrayLayers = 1,
        .imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT | VK_IMAGE_USAGE_TRANSFER_DST_BIT,
        .imageSharingMode = VK_SHARING_MODE_EXCLUSIVE,
        .queueFamilyIndexCount = 0,
        .pQueueFamilyIndices = NULL,
        .preTransform = surfaceCapabilities.currentTransform,
        .compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR,
        .presentMode = window_vk.presentMode,
        .clipped = VK_FALSE,
        .oldSwapchain = VK_NULL_HANDLE
    };

    if (vkCreateSwapchainKHR(graphics_vk.device, &swapchainCreateInfo, NULL, &window_vk.swapchain) != VK_SUCCESS)
    {
        return false;
    }

    return true;
}

void FWindowVulkanDestroySwapchain()
{
    if (window_vk.swapchain != VK_NULL_HANDLE)
    {
        vkDestroySwapchainKHR(graphics_vk.device, window_vk.swapchain, NULL);
    }
}