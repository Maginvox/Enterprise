/* Copyright © 2021 Caden Miller, All Rights Reserved. */

#include <vulkan/vulkan.h>

#include "Core/FMemory.h"
#include "Core/FMath.h"
#include "Core/FBitset.h"
#include "Core/FLog.h"

#include "FVulkanExtensions.h"
#include "FVulkanFormats.h"
#include "FRenderPassVulkan.h"
#include "FFramebufferVulkan.h"
#include "FGraphicsVulkan.h"

FGraphicsVulkan graphics_vk;

VkBool32 FVulkanValidationLogger(VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity,VkDebugUtilsMessageTypeFlagsEXT messageTypes, const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData, void* pUserData);

bool FGraphicsInitialize(const FWindowInfo* pWindowInfo, const FGraphicsOptions* pOptions)
{
    if (!FWindowInitialize(pWindowInfo))
    {
        return false;
    }

    if (pWindowInfo == NULL || pOptions == NULL)
    {
        return false;
    }

    /* Build the info to create the Vulkan instance. */
    VkApplicationInfo applicationInfo = {
        .sType = VK_STRUCTURE_TYPE_APPLICATION_INFO,
        .pNext = NULL,
        .pApplicationName = "EnterpriseApplication",
        .applicationVersion = VK_MAKE_VERSION(0, 0, 1),
        .pEngineName = "Enterprise",
        .engineVersion = VK_MAKE_VERSION(0, 0, 1),
        .apiVersion = VK_API_VERSION_1_1 /* Many devices still only support the Vulkan 1.1 specification. */
    };

    /* Get the validation layers and instance extensions. */
    FInt32 validationLayersCount = 0;
    const char* const* ppValidationLayers = FVulkanValidationLayers(&validationLayersCount);

    FInt32 instanceExtensionsCount = 0;
    const char* const* ppInstanceExtension = FVulkanInstanceExtensions(&instanceExtensionsCount);

#ifdef ENTERPRISE_DEBUG
    VkDebugUtilsMessengerCreateInfoEXT messengerCreateInfo = 
    {
        .sType = VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT,
        .pNext = NULL,
        .flags = 0,
        .messageSeverity = VK_DEBUG_UTILS_MESSAGE_SEVERITY_INFO_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT,
        .messageType = VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT,
        .pfnUserCallback = FVulkanValidationLogger,
        .pUserData = NULL
    };
#endif

    VkInstanceCreateInfo createInfo = {
        .sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO,
#ifdef ENTERPRISE_DEBUG
        .pNext = &messengerCreateInfo,
#else
        .pNext = NULL,
#endif
        .flags = 0,
        .pApplicationInfo = &applicationInfo,
        .enabledLayerCount = validationLayersCount,
        .ppEnabledLayerNames = ppValidationLayers,
        .enabledExtensionCount = instanceExtensionsCount,
        .ppEnabledExtensionNames = ppInstanceExtension
    };

    if (vkCreateInstance(&createInfo, NULL, &graphics_vk.instance) != VK_SUCCESS)
    {
        FGraphicsShutdown();
        FLogError("Could not create the vulkan instance!");
        return false;
    }

    
#ifdef ENTERPRISE_DEBUG
    /* Load the messenger create and destroy functions */
    graphics_vk.pCreateDebugMessenger = (PFN_vkCreateDebugUtilsMessengerEXT)vkGetInstanceProcAddr(graphics_vk.instance, "vkCreateDebugUtilsMessengerEXT");
    graphics_vk.pDestroyDebugMessenger = (PFN_vkDestroyDebugUtilsMessengerEXT)vkGetInstanceProcAddr(graphics_vk.instance, "vkDestroyDebugUtilsMessengerEXT");

    if (graphics_vk.pCreateDebugMessenger == NULL || graphics_vk.pDestroyDebugMessenger == NULL)
    {
        FGraphicsShutdown();
        FLogError("Could not load vulkan debug functions, was the VulkanSDK installed?");
        return false;
    }

    if (graphics_vk.pCreateDebugMessenger(graphics_vk.instance, &messengerCreateInfo, NULL, &graphics_vk.debugMessenger) != VK_SUCCESS)
    {
        FGraphicsShutdown();
        FLogError("Could not create the vulkan debug messenger!");
        return false;
    }
#endif

    /* Now that the surface is created we can create the window surface */
    if (!FWindowVulkanInitialize())
    {
        FGraphicsShutdown();
        FLogError("Could not initialize the vulkan window!");
        return false;
    }

    /* Find the physical device */
    uint32_t physicalDeviceCount = 0;
    if (vkEnumeratePhysicalDevices(graphics_vk.instance, &physicalDeviceCount, NULL) != VK_SUCCESS)
    {
        FGraphicsShutdown();
        FLogError("Could not list the vulkan physical devices!");
        return false;
    }

    VkPhysicalDevice* pPhysicalDevices = FAllocateZero(physicalDeviceCount, sizeof(VkPhysicalDevice));
    if (pPhysicalDevices == NULL)
    {
        FGraphicsShutdown();
        return false;
    }

    vkEnumeratePhysicalDevices(graphics_vk.instance, &physicalDeviceCount, pPhysicalDevices);

    /* Check and see if the physical device option is present */
    if (FMathIsBetween(pOptions->physicalDevice, 0, physicalDeviceCount))
    {
        graphics_vk.physicalDevice = pPhysicalDevices[pOptions->physicalDevice];
    }
    else
    {
        graphics_vk.physicalDevice = pPhysicalDevices[0];
    }
    
    FDeallocate(pPhysicalDevices);


    /* Get the physical devices queues */
    FUInt queueFamilyPropertiesCount = 0;
    vkGetPhysicalDeviceQueueFamilyProperties(graphics_vk.physicalDevice, &queueFamilyPropertiesCount, NULL);

    VkQueueFamilyProperties* pQueueFamilyProperties = FAllocateZero(queueFamilyPropertiesCount, sizeof(VkQueueFamilyProperties));
    if (pQueueFamilyProperties == NULL)
    {
        FGraphicsShutdown();
        return false;
    }

    vkGetPhysicalDeviceQueueFamilyProperties(graphics_vk.physicalDevice, &queueFamilyPropertiesCount, pQueueFamilyProperties);

    /* Loop through all of the device queue families and find the required ones */
    
    /*
          1 2 3 4 5
        G 1 0 1 0 1
        C 0 0 0 1 1
        P 1 1 0 0 1 

        In this case with 5 queue families and we require a graphics family, compute family, and present family.
        We should choose 2, 3, and 4 for optimal preformance.

        Algorithm

            1. Check if the required queue is only found in one queue family. If so choose that family immediatly for the queue.
            2. Try to find a family that is specialized for that task. The family only supports that queue. 
            3. Find a less specialized queue family and use it instead.
    */

    /* Find the graphics queue */
    bool foundGraphicsFamily = false, foundPresentFamily = false, foundComputeFamily = false;
    FUInt32 graphicsFamily = 0, presentFamily = 0, computeFamily = 0;
    
    for (FUInt32 i = 0; i < queueFamilyPropertiesCount; i++)
    {
        VkQueueFamilyProperties* pFamilyProperties = &pQueueFamilyProperties[i];

        /* Check for the graphics family */
        if (pFamilyProperties->queueFlags & VK_QUEUE_GRAPHICS_BIT)
        {
            foundGraphicsFamily = true;

            if (pFamilyProperties->queueFlags == VK_QUEUE_GRAPHICS_BIT) /* This queue is only for graphics */
            {
                graphicsFamily = i;
                continue;
            }

            if (pQueueFamilyProperties[graphicsFamily].queueFlags > pFamilyProperties->queueFlags)
            {
                graphicsFamily = i;
            }
        }

        /* Check for the present family */
        VkBool32 presentSupport = VK_FALSE;
        vkGetPhysicalDeviceSurfaceSupportKHR(graphics_vk.physicalDevice, i, window_vk.surface, &presentSupport);

        if (presentSupport)
        {
            foundPresentFamily = true;

            if (pQueueFamilyProperties[presentFamily].queueFlags > pFamilyProperties->queueFlags)
            {
                presentFamily = i;
            }
        }

        /* Check for the compute family */
        if (pFamilyProperties->queueFlags & VK_QUEUE_COMPUTE_BIT)
        {
            foundComputeFamily = true;

            if (pFamilyProperties->queueFlags == VK_QUEUE_COMPUTE_BIT) /* This queue is only for graphics */
            {
                computeFamily = i;
                continue;
            }

            if (pQueueFamilyProperties[computeFamily].queueFlags > pFamilyProperties->queueFlags)
            {
                computeFamily = i;
            }
        }
    }

    FDeallocate(pQueueFamilyProperties);

    /* Make sure all of the queue families were found. */ 
    if (!foundGraphicsFamily || !foundPresentFamily || !foundComputeFamily)
    {
        FGraphicsShutdown();
        FLogError("Could not find a required vulkan queue!");
        return false;
    }

    /* Look for duplicates in the queue families */
    const bool bGraphicsAndPresentSame = (graphicsFamily == presentFamily);
    const bool bPresentAndComputeSame = (presentFamily == computeFamily);
    const bool bAllQueuesSame = (bGraphicsAndPresentSame && bPresentAndComputeSame);
    const bool bAllQueuesDifferent = (!bGraphicsAndPresentSame && !bPresentAndComputeSame);

    const FUInt32 queueFamilyCount = bAllQueuesSame ? 1 : (bAllQueuesDifferent ? 3 : 2);

    VkDeviceQueueCreateInfo* pQueueCreateInfos = FAllocateZero(queueFamilyCount, sizeof(VkDeviceQueueCreateInfo));
    if (pQueueCreateInfos == NULL)
    {
        FGraphicsShutdown();
        return false;
    }

    float pQueuePriorities[3] = 
    {
        1.0f,
        1.0f,
        1.0f
    };

    pQueueCreateInfos[0].sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
    pQueueCreateInfos[0].pNext = NULL;
    pQueueCreateInfos[0].flags = 0;
    pQueueCreateInfos[0].queueFamilyIndex = graphicsFamily;
    pQueueCreateInfos[0].queueCount = 1;
    pQueueCreateInfos[0].pQueuePriorities = pQueuePriorities;
    FInt queueFamiliesUsed = 1;

    if (!bGraphicsAndPresentSame)
    {
        pQueueCreateInfos[queueFamiliesUsed].sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
        pQueueCreateInfos[queueFamiliesUsed].pNext = NULL;
        pQueueCreateInfos[queueFamiliesUsed].flags = 0;
        pQueueCreateInfos[queueFamiliesUsed].queueFamilyIndex = presentFamily;
        pQueueCreateInfos[queueFamiliesUsed].queueCount = 1;
        pQueueCreateInfos[queueFamiliesUsed].pQueuePriorities = pQueuePriorities;
        queueFamiliesUsed++;
    }

    if (!bPresentAndComputeSame)
    {
        pQueueCreateInfos[queueFamiliesUsed].sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
        pQueueCreateInfos[queueFamiliesUsed].pNext = NULL;
        pQueueCreateInfos[queueFamiliesUsed].flags = 0;
        pQueueCreateInfos[queueFamiliesUsed].queueFamilyIndex = computeFamily;
        pQueueCreateInfos[queueFamiliesUsed].queueCount = 1;
        pQueueCreateInfos[queueFamiliesUsed].pQueuePriorities = pQueuePriorities;
        queueFamiliesUsed++;
    }

    /* Get the device extensions. */ 
    FInt32 deviceExtensionsCount = 0;
    const char* const* ppDeviceExtension = FVulkanDeviceExtensions(graphics_vk.physicalDevice, &deviceExtensionsCount);

    const VkPhysicalDeviceFeatures deviceFeatures = {0};
    const VkDeviceCreateInfo deviceCreateInfo = 
    {
        .sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO,
        .pNext = NULL,
        .flags = 0,
        .queueCreateInfoCount = queueFamilyCount,
        .pQueueCreateInfos = pQueueCreateInfos,
        .enabledLayerCount = validationLayersCount,
        .ppEnabledLayerNames = ppValidationLayers,
        .enabledExtensionCount = deviceExtensionsCount,
        .ppEnabledExtensionNames = ppDeviceExtension,
        .pEnabledFeatures = &deviceFeatures
    };

    if (vkCreateDevice(graphics_vk.physicalDevice, &deviceCreateInfo, NULL, &graphics_vk.device) != VK_SUCCESS)
    {
        FDeallocate(pQueueCreateInfos);
        FGraphicsShutdown();
        FLogError("Could not create the Vulkan device!");
        return false;
    }

    FDeallocate(pQueueCreateInfos);

    /* We can now create the main windows swapchain */
    if (!FWindowVulkanCreateSwapchain())
    {
        FGraphicsShutdown();
        return false;
    }

    return true;
}

void FGraphicsShutdown()
{
    FWindowVulkanShutdown();

    if (graphics_vk.device != VK_NULL_HANDLE)
    {
        vkDestroyDevice(graphics_vk.device, NULL);
    }

#ifdef ENTERPRISE_DEBUG
    if (graphics_vk.debugMessenger != VK_NULL_HANDLE)
    {
        graphics_vk.pDestroyDebugMessenger(graphics_vk.instance, graphics_vk.debugMessenger, NULL);
    }
#endif

    if (graphics_vk.instance != VK_NULL_HANDLE)
    {
        vkDestroyInstance(graphics_vk.instance, NULL);
    }

    FWindowShutdown();
}

VkBool32 FVulkanValidationLogger(VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity,VkDebugUtilsMessageTypeFlagsEXT messageTypes, const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData, void* pUserData)
{
    switch(messageSeverity)
    {
        case VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT:
        case VK_DEBUG_UTILS_MESSAGE_SEVERITY_INFO_BIT_EXT:
            FLogInfo(pCallbackData->pMessage);
            break;
        case VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT:
            FLogWarning(pCallbackData->pMessage);
            break;
        case VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT:
            FLogError(pCallbackData->pMessage);
        break;
    }

    return VK_FALSE;
}
