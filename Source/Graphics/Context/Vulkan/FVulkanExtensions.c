#include <vulkan/vulkan.h>
#include <SDL.h>

#include "Core/FTypes.h"
#include "Core/FMacro.h"
#include "Core/FMemory.h"
#include "Core/FString.h"
#include "Graphics/Context/Vulkan/FVulkanExtensions.h"

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
    FInt32 propertiesCount = 0;
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
    for (int i = 0; i < FCOUNT_OF(ppValidationLayers); i++)
    {
        bool found = false;

        for (int j = 0; j < propertiesCount; j++)
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

const char* const* FVulkanInstanceExtensions(FInt32* pCount)
{
    return NULL;
}