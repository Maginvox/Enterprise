#include "Core/FMemory.h"
#include "Graphics/Context/Vulkan/FRenderContextVulkan.h"
#include "Graphics/Context/Vulkan/FVulkanExtensions.h"

FRenderContext* FRenderContextVulkan_Create(const FRenderContextCreateInfo* pInfo)
{

    FRenderContextVulkan* pContextVulkan = FAllocateZero(1, sizeof(FRenderContextVulkan));
    if (pContextVulkan == NULL)
    {
        return NULL;
    }

    FRenderContext* pContext = (FRenderContext*)pContextVulkan;

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
        FRenderContextVulkan_Destroy(&pContext);
        return NULL;
    }

    /* Find the physical device */
    FInt32 physicalDeviceCount = 0;
    if (vkEnumeratePhysicalDevices(pContextVulkan->instance, &physicalDeviceCount, NULL) != VK_SUCCESS)
    {
        FRenderContextVulkan_Destroy(&pContext);
        return NULL;
    }

    VkPhysicalDevice* pPhysicalDevices = FAllocateZero(physicalDeviceCount, sizeof(VkPhysicalDevice));
    if (pPhysicalDevices == NULL)
    {
        FRenderContextVulkan_Destroy(&pContext);
        return NULL;
    }

    vkEnumeratePhysicalDevices(pContextVulkan->instance, &physicalDeviceCount, pPhysicalDevices);

    return pContext;
}

void FRenderContextVulkan_Destroy(FRenderContext** ppContext)
{
    if (ppContext == NULL || *ppContext == NULL)
    {
        return;
    }

    FRenderContextVulkan* pContextVulkan = (FRenderContextVulkan*)*ppContext;

    if (pContextVulkan->instance != VK_NULL_HANDLE)
    {
        vkDestroyInstance(pContextVulkan->instance, NULL);
    }

    FDeallocate(pContextVulkan);
    *ppContext = NULL;
}