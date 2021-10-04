/* Copyright © 2021 Caden Miller, All Rights Reserved. */

#include <vulkan/vulkan.h>

#include "Core/enMacro.h"
#include "Core/enMemory.h"
#include "Core/enMath.h"
#include "Core/enBitset.h"
#include "Core/enLog.h"

#include "enVulkanExtensions.h"
#include "enVulkanFormats.h"
#include "enGraphicsVulkan.h"

enGraphicsVulkan graphics_vk;

VkBool32 enVulkanValidationLogger(VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity,VkDebugUtilsMessageTypeFlagsEXT messageTypes, const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData, void* pUserData);

bool enGraphicsInitialize(const enWindowInfo* pWindowInfo, const enGraphicsOptions* pOptions)
{
    if (!enWindowInitialize(pWindowInfo))
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
    int32 validationLayersCount = 0;
    const char* const* ppValidationLayers = enVulkanValidationLayers(&validationLayersCount);

    int32 instanceExtensionsCount = 0;
    const char* const* ppInstanceExtension = enVulkanInstanceExtensions(&instanceExtensionsCount);

#ifdef ENTERPRISE_DEBUG
    VkDebugUtilsMessengerCreateInfoEXT messengerCreateInfo = 
    {
        .sType = VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT,
        .pNext = NULL,
        .flags = 0,
        .messageSeverity = VK_DEBUG_UTILS_MESSAGE_SEVERITY_INFO_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT,
        .messageType = VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT,
        .pfnUserCallback = enVulkanValidationLogger,
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
        enGraphicsShutdown();
        enLogError("Could not create the vulkan instance!");
        return false;
    }

    
#ifdef ENTERPRISE_DEBUG
    /* Load the messenger create and destroy functions */
    graphics_vk.pCreateDebugMessenger = (PFN_vkCreateDebugUtilsMessengerEXT)vkGetInstanceProcAddr(graphics_vk.instance, "vkCreateDebugUtilsMessengerEXT");
    graphics_vk.pDestroyDebugMessenger = (PFN_vkDestroyDebugUtilsMessengerEXT)vkGetInstanceProcAddr(graphics_vk.instance, "vkDestroyDebugUtilsMessengerEXT");

    if (graphics_vk.pCreateDebugMessenger == NULL || graphics_vk.pDestroyDebugMessenger == NULL)
    {
        enGraphicsShutdown();
        enLogError("Could not load vulkan debug functions, was the VulkanSDK installed?");
        return false;
    }

    if (graphics_vk.pCreateDebugMessenger(graphics_vk.instance, &messengerCreateInfo, NULL, &graphics_vk.debugMessenger) != VK_SUCCESS)
    {
        enGraphicsShutdown();
        enLogError("Could not create the vulkan debug messenger!");
        return false;
    }
#endif

    /* Now that the surface is created we can create the window surface */
    if (!enWindowVulkanInitialize())
    {
        enGraphicsShutdown();
        enLogError("Could not initialize the vulkan window!");
        return false;
    }

    /* Find the physical device */
    uint32_t physicalDeviceCount = 0;
    if (vkEnumeratePhysicalDevices(graphics_vk.instance, &physicalDeviceCount, NULL) != VK_SUCCESS)
    {
        enGraphicsShutdown();
        enLogError("Could not list the vulkan physical devices!");
        return false;
    }

    VkPhysicalDevice* pPhysicalDevices = enAllocateZero(physicalDeviceCount, sizeof(VkPhysicalDevice));
    if (pPhysicalDevices == NULL)
    {
        enGraphicsShutdown();
        return false;
    }

    vkEnumeratePhysicalDevices(graphics_vk.instance, &physicalDeviceCount, pPhysicalDevices);

    /* Check and see if the physical device option is present */
    if (enMathIsBetween(pOptions->physicalDevice, 0, physicalDeviceCount))
    {
        graphics_vk.physicalDevice = pPhysicalDevices[pOptions->physicalDevice];
    }
    else
    {
        graphics_vk.physicalDevice = pPhysicalDevices[0];
    }
    
    enDeallocate(pPhysicalDevices);


    /* Get the physical devices queues */
    uint32_t queueFamilyPropertiesCount = 0;
    vkGetPhysicalDeviceQueueFamilyProperties(graphics_vk.physicalDevice, &queueFamilyPropertiesCount, NULL);

    VkQueueFamilyProperties* pQueueFamilyProperties = enAllocateZero(queueFamilyPropertiesCount, sizeof(VkQueueFamilyProperties));
    if (pQueueFamilyProperties == NULL)
    {
        enGraphicsShutdown();
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
    uint32 graphicsFamily = 0, presentFamily = 0, computeFamily = 0;
    
    for (uint32 i = 0; i < queueFamilyPropertiesCount; i++)
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

    enDeallocate(pQueueFamilyProperties);

    /* Make sure all of the queue families were found. */ 
    if (!foundGraphicsFamily || !foundPresentFamily || !foundComputeFamily)
    {
        enGraphicsShutdown();
        enLogError("Could not find a required vulkan queue!");
        return false;
    }

    /* Look for duplicates in the queue families */
    const bool bGraphicsAndPresentSame = (graphicsFamily == presentFamily);
    const bool bPresentAndComputeSame = (presentFamily == computeFamily);
    const bool bAllQueuesSame = (bGraphicsAndPresentSame && bPresentAndComputeSame);
    const bool bAllQueuesDifferent = (!bGraphicsAndPresentSame && !bPresentAndComputeSame);

    const uint32 queueFamilyCount = bAllQueuesSame ? 1 : (bAllQueuesDifferent ? 3 : 2);

    VkDeviceQueueCreateInfo* pQueueCreateInfos = enAllocateZero(queueFamilyCount, sizeof(VkDeviceQueueCreateInfo));
    if (pQueueCreateInfos == NULL)
    {
        enGraphicsShutdown();
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
    uint32 queueFamiliesUsed = 1;

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
    int32 deviceExtensionsCount = 0;
    const char* const* ppDeviceExtension = enVulkanDeviceExtensions(graphics_vk.physicalDevice, &deviceExtensionsCount);

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
        enDeallocate(pQueueCreateInfos);
        enGraphicsShutdown();
        enLogError("Could not create the Vulkan device!");
        return false;
    }

    enDeallocate(pQueueCreateInfos);

    /* We can now create the main windows swapchain */
    if (!enWindowVulkanCreateSwapchain())
    {
        enGraphicsShutdown();
        return false;
    }

    /*
        Color,
        Specular,
        WorldNormal,
        Roughness
        Depth,
    */     

    VkAttachmentDescription generalAttachments[1] = 
    {
        { /* Color Attachment */
            .flags = 0,
            .format = window_vk.surfaceFormat.format,
            .samples = VK_SAMPLE_COUNT_1_BIT,
            .loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR,
            .storeOp = VK_ATTACHMENT_STORE_OP_STORE,
            .stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE,
            .stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE,
            .initialLayout = VK_IMAGE_LAYOUT_UNDEFINED,
            .finalLayout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL
        }
    };

    VkAttachmentReference generalAttachmentReferences[1] =
    {
        { /* Color Reference */
            .attachment = 0,
            .layout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL
        }
    };

    VkSubpassDescription generalSubpasses[1] =
    {
        { /* Color Subpass */
            .flags = 0,
            .pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS,
            .inputAttachmentCount = 0,
            .pInputAttachments = NULL,
            .colorAttachmentCount = 1,
            .pColorAttachments = (VkAttachmentReference[1])
                {
                    generalAttachmentReferences[0]
                },
            .pResolveAttachments = NULL,
            .pDepthStencilAttachment = NULL,
            .preserveAttachmentCount = 0,
            .pPreserveAttachments = NULL
        }
    };

    VkSubpassDependency generalDependencies[1] =
    {
        {
            .srcSubpass = VK_SUBPASS_EXTERNAL,
            .dstSubpass = 0,
            .srcStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT,
            .srcAccessMask = 0,
            .dstStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT,
            .dstAccessMask = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT
        }
    };

    VkRenderPassCreateInfo generalPassCreateInfo =
    {
        .sType = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO,
        .pNext = NULL,
        .flags = 0,
        .attachmentCount = COUNT_OF(generalAttachments),
        .pAttachments = generalAttachments,
        .subpassCount = COUNT_OF(generalSubpasses),
        .pSubpasses = generalSubpasses,
        .dependencyCount = COUNT_OF(generalDependencies),
        .pDependencies = generalDependencies,
    };

    if (vkCreateRenderPass(graphics_vk.device, &generalPassCreateInfo, NULL, &graphics_vk.generalPass) != VK_SUCCESS)
    {
        enGraphicsShutdown();
        enLogError("Could not create the general render pass!");
        return false;
    }

    /* Create the global descriptor set */
    VkDescriptorSetLayoutBinding globalBinding =
    {
        .binding = 0,
        .descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER,
        .descriptorCount = 1,
        .stageFlags = VK_SHADER_STAGE_ALL_GRAPHICS,
        .pImmutableSamplers = NULL
    };

    VkDescriptorSetLayoutCreateInfo globalLayoutCreateInfo =
    {
        .sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO,
        .pNext = NULL,
        .flags = 0,
        .bindingCount = 1,
        .pBindings = &globalBinding
    };

    if (vkCreateDescriptorSetLayout(graphics_vk.device, &globalLayoutCreateInfo, NULL, &graphics_vk.globalLayout) != VK_SUCCESS)
    {
        enGraphicsShutdown();
        enLogError("Could not create the global descriptor layout!");
        return false;
    }
    
    return true;
}

void enGraphicsShutdown()
{
    if (graphics_vk.globalLayout != VK_NULL_HANDLE)
    {
        vkDestroyDescriptorSetLayout(graphics_vk.device, graphics_vk.globalLayout, NULL);
    }

    if (graphics_vk.generalPass != VK_NULL_HANDLE)
    {
        vkDestroyRenderPass(graphics_vk.device, graphics_vk.generalPass, NULL);
    }

    enWindowVulkanShutdown();

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

    enWindowShutdown();
}

VkBool32 enVulkanValidationLogger(VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity,VkDebugUtilsMessageTypeFlagsEXT messageTypes, const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData, void* pUserData)
{
    switch(messageSeverity)
    {
        case VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT:
        case VK_DEBUG_UTILS_MESSAGE_SEVERITY_INFO_BIT_EXT:
            enLogInfo(pCallbackData->pMessage);
            break;
        case VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT:
            enLogWarning(pCallbackData->pMessage);
            break;
        case VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT:
            enLogError(pCallbackData->pMessage);
        break;
    }

    return VK_FALSE;
}
