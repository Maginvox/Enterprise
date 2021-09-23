/* Copyright © 2021 Caden Miller, All Rights Reserved. */

#include "Core/FMemory.h"
#include "Core/FMath.h"
#include "Core/FBitset.h"

#include "FVulkanExtensions.h"
#include "FGraphicsContextVulkan.h"
#include "FRenderPassVulkan.h"
#include "FFramebufferVulkan.h"
#include "FWindowVulkan.h"

#include "../../FGraphicsContext_Impl.h"

void FGraphicsContextVulkanLoad(FGraphicsContext* pContext)
{
    pContext->pRenderContextDestroy = FGraphicsContextVulkanDestroy;
    
    pContext->pRenderPassCreate = FRenderPassVulkanCreate;
    pContext->pRenderPassDestroy = FRenderPassVulkanDestroy;

    pContext->pFramebufferCreate = FFramebufferVulkanCreate;
    pContext->pFramebufferDestroy = FFramebufferVulkanDestroy;
}

FGraphicsContext* FGraphicsContextVulkanCreate(FWindow* pWindow, const FGraphicsOptions* pOptions)
{
    FWindowSystemSetCreateCallback(FVulkanWindowCreateCallback);
    FWindowSystemSetDestroyCallback(FVulkanWindowDestroyCallback);

    FGraphicsContextVulkan* pContextVulkan = FAllocateZero(1, sizeof(FGraphicsContextVulkan));
    if (pContextVulkan == NULL)
    {
        return NULL;
    }

    FGraphicsContext* pContext = (FGraphicsContext*)pContextVulkan;

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
    const char* const* ppInstanceExtensions = FVulkanInstanceExtensions(&instanceExtensionsCount);

    VkInstanceCreateInfo createInfo = {
        .sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO,
        .pNext = NULL,
        .flags = 0,
        .pApplicationInfo = &applicationInfo,
        .enabledLayerCount = validationLayersCount,
        .ppEnabledLayerNames = ppValidationLayers,
        .enabledExtensionCount = instanceExtensionsCount,
        .ppEnabledExtensionNames = ppInstanceExtensions
    };

    if (vkCreateInstance(&createInfo, NULL, &pContextVulkan->instance) != VK_SUCCESS)
    {
        FGraphicsContextVulkanDestroy(&pContext);
        return NULL;
    }

    /* Now that the surface is created we can create the window surface */
    FWindowVulkan* pWindowVulkan = FWindowVulkanCreate(pWindow);
    if (pWindowVulkan == NULL)
    {
        FGraphicsContextVulkanDestroy(&pContext);
        return NULL;
    }

    FWindowSetRenderData(pWindow, pWindowVulkan);

    /* Find the physical device */
    uint32_t physicalDeviceCount = 0;
    if (vkEnumeratePhysicalDevices(pContextVulkan->instance, &physicalDeviceCount, NULL) != VK_SUCCESS)
    {
        FGraphicsContextVulkanDestroy(&pContext);
        return NULL;
    }

    VkPhysicalDevice* pPhysicalDevices = FAllocateZero(physicalDeviceCount, sizeof(VkPhysicalDevice));
    if (pPhysicalDevices == NULL)
    {
        FGraphicsContextVulkanDestroy(&pContext);
        return NULL;
    }

    vkEnumeratePhysicalDevices(pContextVulkan->instance, &physicalDeviceCount, pPhysicalDevices);

    /* Check and see if the physical device option is present */
    if (FMathIsBetween(pOptions->physicalDevice, 0, physicalDeviceCount))
    {
        pContextVulkan->physicalDevice = pPhysicalDevices[pOptions->physicalDevice];
    }
    else
    {
        pContextVulkan->physicalDevice = pPhysicalDevices[0];
    }
    
    FDeallocate(pPhysicalDevices);


    /* Get the physical devices queues */
    FUInt32 queueFamilyPropertiesCount = 0;
    vkGetPhysicalDeviceQueueFamilyProperties(pContextVulkan->physicalDevice, &queueFamilyPropertiesCount, NULL);

    VkQueueFamilyProperties* pQueueFamilyProperties = FAllocateZero(queueFamilyPropertiesCount, sizeof(VkQueueFamilyProperties));
    if (pQueueFamilyProperties == NULL)
    {
        FGraphicsContextVulkanDestroy(&pContext);
        return NULL;
    }

    vkGetPhysicalDeviceQueueFamilyProperties(pContextVulkan->physicalDevice, &queueFamilyPropertiesCount, pQueueFamilyProperties);

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

    #define FQUEUE_COUNT 3

    
    VkQueueFlags requestedQueues

    for (FInt i = 0; i < FQUEUE_COUNT; i++)
    {
        FInt32 bestQueue = 0;

        for (FUInt32 j = 0; j < queueFamilyPropertiesCount; j++)
        {
            char bits[FBITNSLOTS(FQUEUE_COUNT)];

            if (pQueueFamilyProperties[j].queueFlags & VK_QUEUE_GRAPHICS_BIT)
            {
                bits[]
            }
        }
    }

    for (FUInt32 i = 0; i < queueFamilyPropertiesCount; i++)
    {
        if (pQueueFamilyProperties[i].queueFlags & VK_QUEUE_GRAPHICS_BIT)
        {
            pContextVulkan->graphicsFamilyIndex = i;
        }

        VkBool32 surfaceSupported = false;
        vkGetPhysicalDeviceSurfaceSupportKHR(pContextVulkan->physicalDevice, i, pWindowData->surface, &surfaceSupported);
        if (surfaceSupported)
        {
            pContextVulkan->presentFamilyIndex = i;
        }
    }


    return pContext;
}

void FGraphicsContextVulkanDestroy(FGraphicsContext** ppContext)
{
    if (ppContext == NULL || *ppContext == NULL)
    {
        return;
    }

    FGraphicsContextVulkan* pContextVulkan = (FGraphicsContextVulkan*)*ppContext;

    if (pContextVulkan->instance != VK_NULL_HANDLE)
    {
        vkDestroyInstance(pContextVulkan->instance, NULL);
    }

    FDeallocate(pContextVulkan);
    *ppContext = NULL;
}