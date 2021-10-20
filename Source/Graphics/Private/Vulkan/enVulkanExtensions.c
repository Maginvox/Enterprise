/* Copyright © 2021 Caden Miller, All Rights Reserved. */

#include <vulkan/vulkan.h>

#include "Core/enTypes.h"
#include "Core/enMacro.h"
#include "Core/enMemory.h"
#include "Core/enString.h"
#include "enVulkanExtensions.h"

static const char* ppValidationLayers[1] = {
    "VK_LAYER_KHRONOS_validation"
};

static const char* ppDeviceExtensions[1] =
{
    VK_KHR_SWAPCHAIN_EXTENSION_NAME
};

#ifdef ENTERPRISE_DEBUG
    #define INSTANCE_EXTENSIONS_COUNT 3
#else
    #define INSTANCE_EXTENSIONS_COUNT 2
#endif

static const char* ppInstanceExtensions[INSTANCE_EXTENSIONS_COUNT] = {
    VK_KHR_SURFACE_EXTENSION_NAME,

    #ifdef ENTERPRISE_DEBUG
        VK_EXT_DEBUG_UTILS_EXTENSION_NAME,
    #endif

    #ifdef ENTERPRISE_WINDOWS
        "VK_KHR_win32_surface"
    #elif ENTERPRISE_MACOS
        "VK_MVK_macos_surface"
    #elif ENTERPRISE_ANDROID
        "VK_KHR_android_surface"
    #elif ENTERPRISE_LINUX
        "VK_KHR_xcb_surface"
    #else
        #error Unsupported graphics platform!
    #endif
};

const char* const* enVulkanValidationLayers(uint32* pCount)
{

    if (pCount == NULL)
    {
        return NULL;
    }

    *pCount = 0;

    /* Get all of the available layers. */
    uint32_t propertiesCount = 0;
    if (vkEnumerateInstanceLayerProperties(&propertiesCount, NULL) != VK_SUCCESS)
    {
        return NULL;
    }

    VkLayerProperties* pProperties = enCalloc(propertiesCount, sizeof(VkLayerProperties));
    if (pProperties == NULL)
    {
        return NULL;
    }

    if (vkEnumerateInstanceLayerProperties(&propertiesCount, pProperties) != VK_SUCCESS)
    {
        enFree(pProperties);
        return NULL;
    }

    /* Loop through all the layers and find out if any are missing. */
    for (uint32 i = 0; i < COUNT_OF(ppValidationLayers); i++)
    {
        bool found = false;

        for (uint32 j = 0; j < propertiesCount; j++)
        {
            if (enStringCompare(pProperties[j].layerName, ppValidationLayers[i], VK_MAX_EXTENSION_NAME_SIZE) == 0)
            {
                found = true;
                break;
            }
        }

        /* If any are missing return NULL */
        if (!found)
        {
            enFree(pProperties);
            return NULL;
        }
    }

    *pCount = COUNT_OF(ppValidationLayers);

    /* None missing, return the string array. */
    return (const char* const*)ppValidationLayers;
}

const char* const* enVulkanInstanceExtensions(uint32* pCount)
{

    if (pCount == NULL)
    {
        return NULL;
    }

    *pCount = 0; 
    
    /* Get the device supported instance extensions */
    uint32_t propertiesCount = 0;
    if (vkEnumerateInstanceExtensionProperties(NULL, &propertiesCount, NULL) != VK_SUCCESS)
    {
        return NULL;
    }

    VkExtensionProperties* pProperties = enCalloc(propertiesCount, sizeof(VkExtensionProperties));
    if (pProperties == NULL)
    {
        return NULL;
    }

    vkEnumerateInstanceExtensionProperties(NULL, &propertiesCount, pProperties);

    /* Loop over all of the requested extensions and make sure they were found */
    for (int32 i = 0; i < COUNT_OF(ppInstanceExtensions); i++)
    {
        bool found = false;

        for (uint32_t j = 0; j < propertiesCount; j++)
        {
            if (enStringCompare(ppInstanceExtensions[i], pProperties[j].extensionName, VK_MAX_EXTENSION_NAME_SIZE) == 0)
            {
                found = true;
                break;
            }
        }

        if (!found)
        {
            enFree(pProperties);
            return NULL;
        }
    }

    *pCount = COUNT_OF(ppInstanceExtensions);

    return (const char* const*)ppInstanceExtensions;
}

const char* const* enVulkanDeviceExtensions(VkPhysicalDevice physicalDevice, uint32* pCount)
{


    if (pCount == NULL)
    {
        return NULL;
    }

    *pCount = 0; 
    
    /* Get the supported device extensions */
    uint32_t propertiesCount = 0;
    if (vkEnumerateDeviceExtensionProperties(physicalDevice, NULL, &propertiesCount, NULL) != VK_SUCCESS)
    {
        return NULL;
    }

    VkExtensionProperties* pProperties = enCalloc(propertiesCount, sizeof(VkExtensionProperties));
    if (pProperties == NULL)
    {
        return NULL;
    }

    vkEnumerateDeviceExtensionProperties(physicalDevice, NULL, &propertiesCount, pProperties);

    /* Loop over all of the requested extensions and make sure they were found */
    for (int32 i = 0; i < COUNT_OF(ppDeviceExtensions); i++)
    {
        bool found = false;

        for (uint32_t j = 0; j < propertiesCount; j++)
        {
            if (enStringCompare(ppDeviceExtensions[i], pProperties[j].extensionName, VK_MAX_EXTENSION_NAME_SIZE) == 0)
            {
                found = true;
                break;
            }
        }

        if (!found)
        {
            enFree(pProperties);
            return NULL;
        }
    }

    *pCount = COUNT_OF(ppDeviceExtensions);

    return (const char* const*)ppDeviceExtensions;
}