/* Copyright © 2021 Caden Miller, All Rights Reserved. */

#include <vulkan/vulkan.h>

#include "Core/FTypes.h"
#include "Core/FMacro.h"
#include "Core/FMemory.h"
#include "Core/FString.h"
#include "FVulkanExtensions.h"

static const char ppValidationLayers[1][VK_MAX_EXTENSION_NAME_SIZE] = {
    "VK_LAYER_KHRONOS_validation"
};

const char* const* FVulkanValidationLayers(FInt32* pCount)
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

    VkLayerProperties* pProperties = FAllocateZero(propertiesCount, sizeof(VkLayerProperties));
    if (pProperties == NULL)
    {
        return NULL;
    }

    if (vkEnumerateInstanceLayerProperties(&propertiesCount, pProperties) != VK_SUCCESS)
    {
        FDeallocate(pProperties);
        return NULL;
    }

    /* Loop through all the layers and find out if any are missing. */
    for (FInt i = 0; i < FCOUNT_OF(ppValidationLayers); i++)
    {
        bool found = false;

        for (FUInt32 j = 0; j < propertiesCount; j++)
        {
            if (FStringCompare(pProperties[j].layerName, VK_MAX_EXTENSION_NAME_SIZE, ppValidationLayers[i], VK_MAX_EXTENSION_NAME_SIZE) == 0)
            {
                found = true;
                break;
            }
        }

        /* If any are missing return NULL */
        if (!found)
        {
            FDeallocate(pProperties);
            return NULL;
        }
    }

    /* None missing, return the string array. */
    return (const char* const*)ppValidationLayers;
}

static const char ppInstanceExtensions[2][VK_MAX_EXTENSION_NAME_SIZE] = {
    VK_KHR_SURFACE_EXTENSION_NAME,

    #ifdef ENTERPRISE_CORE_WINDOWS
        "VK_KHR_win32_surface"
    #elif ENTERPRISE_CORE_MACOS
        "VK_MVK_macos_surface"
    #elif ENTERPRISE_CORE_ANDROID
        "VK_KHR_android_surface"
    #elif ENTERPRISE_CORE_LINUX
        #ifdef ENTERPRISE_GRAPHICS_X11
            "VK_KHR_xlib_surface"
        #elif ENTERPRISE_GRAPHICS_WAYLAND
            "VK_KHR_wayland_surface"
        #else
            #error Unsupported linux graphics platform!
        #endif
    #else
        #error Unsupported graphics platform!
    #endif
};

const char* const* FVulkanInstanceExtensions(FInt32* pCount)
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

    VkExtensionProperties* pProperties = FAllocateZero(propertiesCount, sizeof(VkExtensionProperties));
    if (pProperties == NULL)
    {
        return NULL;
    }

    vkEnumerateInstanceExtensionProperties(NULL, &propertiesCount, pProperties);

    /* Loop over all of the requested extensions and make sure they were found */
    for (FInt32 i = 0; i < FCOUNT_OF(ppInstanceExtensions); i++)
    {
        bool found = false;

        for (uint32_t j = 0; j < propertiesCount; j++)
        {
            if (FStringCompare(ppInstanceExtensions[i], VK_MAX_EXTENSION_NAME_SIZE, pProperties[j].extensionName, VK_MAX_EXTENSION_NAME_SIZE) == 0)
            {
                found = true;
                break;
            }
        }

        if (!found)
        {
            FDeallocate(pProperties);
            return NULL;
        }
    }

    return (const char* const*)ppInstanceExtensions;
}