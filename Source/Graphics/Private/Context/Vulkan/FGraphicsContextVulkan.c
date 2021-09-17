#include "Core/FMemory.h"
#include "FVulkanExtensions.h"
#include "FGraphicsContextVulkan.h"
#include "FRenderPassVulkan.h"
#include "FFramebufferVulkan.h"

#include "../../FGraphicsContext_Impl.h"

void FGraphicsContextVulkanLoad(FGraphicsContext* pContext)
{
    pContext->pRenderContextDestroy = FGraphicsContextVulkanDestroy;
    
    pContext->pRenderPassCreate = FRenderPassVulkanCreate;
    pContext->pRenderPassDestroy = FRenderPassVulkanDestroy;

    pContext->pFramebufferCreate = FFramebufferVulkanCreate;
    pContext->pFramebufferDestroy = FFramebufferVulkanDestroy;
}

FGraphicsContext* FGraphicsContextVulkanCreate()
{

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