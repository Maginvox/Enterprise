#include "Core/FLog.h"
#include "Core/FMemory.h"
#include "Graphics/Vulkan/FVulkanUtils.h"
#include "Graphics/Vulkan/FRenderDeviceVulkan.h"

#include <SDL_vulkan.h>

void FRenderVulkanLoad(FRenderer* pRenderer)
{
	pRenderer->api = E_RENDER_API_VULKAN;
	pRenderer->pRenderDeviceDestroy = FRenderDeviceVulkanDestroy;
	pRenderer->pTextureCreate = FTextureVulkanCreate;
	pRenderer->pTextureDestroy = FTextureVulkanDestroy;
	pRenderer->pMeshCreate = FMeshVulkanCreate;
	pRenderer->pMeshDestroy = FMeshVulkanDestroy;
	pRenderer->pMaterialCreate = FMaterialVulkanCreate;
	pRenderer->pMaterialDestroy = FMaterialVulkanDestroy;
	pRenderer->pMaterialInstanceCreate = FMaterialInstanceVulkanCreate;
	pRenderer->pMaterialInstanceDestroy = FMaterialInstanceVulkanDestroy;
	pRenderer->pMaterialInstanceSetFloat = FMaterialInstanceVulkanSetFloat;
	pRenderer->pMaterialInstanceSetVector2 = FMaterialInstanceVulkanSetVector2;
	pRenderer->pMaterialInstanceSetVector3 = FMaterialInstanceVulkanSetVector3;
	pRenderer->pMaterialInstanceSetVector4 = FMaterialInstanceVulkanSetVector4;
	pRenderer->pMaterialInstanceSetMatrix4 = FMaterialInstanceVulkanSetMatrix4;
	pRenderer->pMaterialInstanceSetTexture = FMaterialInstanceVulkanSetTexture;

	pRenderer->pRenderDeviceSurfaceResizeCallback = FRenderDeviceVulkanSurfaceResizeCallback;
}

bool FRenderDeviceVulkanCreate(FRenderer* pRenderer)
{
	if (pRenderer == NULL)
	{
		return false;
	}

	FRenderDeviceVulkan* pRenderDeviceVulkan = FAllocateZero(1, sizeof(FRenderDeviceVulkan));
	if (pRenderDeviceVulkan == NULL)
	{
		return false;
	}

	FRenderDevice* pRenderDevice = (FRenderDevice*)pRenderDeviceVulkan;
	pRenderer->pRenderDevice = pRenderDevice;

	VkExtent2D extent = {pRenderer->width, pRenderer->height};
	bool debug = pRenderer->options.bDebugAndValidate;
	if (!FRenderDeviceVulkanCreateInstance(pRenderDeviceVulkan, debug) ||
		!FRenderDeviceVulkanCreateSurface(pRenderer, pRenderDeviceVulkan) ||
		!FRenderDeviceVulkanChoosePhysicalDevice(pRenderDeviceVulkan, pRenderer->options.physicalDeviceVulkanID) ||
		!FRenderDeviceVulkanChooseFormats(pRenderDeviceVulkan) ||
		!FRenderDeviceVulkanChooseQueueFamilies(pRenderDeviceVulkan) ||
		!FRenderDeviceVulkanCreateDevice(pRenderDeviceVulkan, debug) ||
		!FRenderDeviceVulkanCreatePools(pRenderDeviceVulkan) ||
		!FRenderDeviceVulkanCreateRenderpasses(pRenderDeviceVulkan) ||
		!FRenderDeviceVulkanCreateSwapchain(pRenderDeviceVulkan, extent))
	{
		FRenderDeviceVulkanDestroy(&pRenderDevice);
		return false;
	}

	pRenderer->pRenderDevice = pRenderDevice;
	return true;
}

void FRenderDeviceVulkanDestroy(FRenderDevice** ppRenderDevice)
{
	if (ppRenderDevice == NULL || *ppRenderDevice == NULL)
	{
		return;
	}

	FRenderDevice* pRenderDevice = (FRenderDevice*)*ppRenderDevice;
	FRenderDeviceVulkan* pRenderDeviceVulkan = (FRenderDeviceVulkan*)pRenderDevice;

	FRenderDeviceVulkanDestroySwapchain(pRenderDeviceVulkan);
	FRenderDeviceVulkanDestroyRenderpasses(pRenderDeviceVulkan);
	FRenderDeviceVulkanDestroyPools(pRenderDeviceVulkan);
	FRenderDeviceVulkanDestroyDevice(pRenderDeviceVulkan);
	FRenderDeviceVulkanDestroySurface(pRenderDeviceVulkan);
	FRenderDeviceVulkanDestroyInstance(pRenderDeviceVulkan);

	FDeallocate(pRenderDevice);
	*ppRenderDevice = NULL;
}


FTexture* FTextureVulkanCreate(FRenderDevice* pRenderDevice, const FTextureCreateOptions* pOptions)
{
	if (pRenderDevice == NULL)
	{
		return NULL;
	}

	FRenderDeviceVulkan* pRenderDeviceVulkan = (FRenderDeviceVulkan*)pRenderDevice;

	/* Allocate the texture */
	FTextureVulkan* pTextureVulkan = FAllocateZero(1, sizeof(FTextureVulkan));
	if (pTextureVulkan == NULL)
	{
		return NULL;
	}
	FTexture* pTexture = (FTexture*)pTextureVulkan;

	/* Create the staging buffer */
	FInt imageSize = pOptions->width * pOptions->height * 4;

	FBufferVulkan* pStagingBuffer = FRenderDeviceVulkanCreateBuffer(pRenderDeviceVulkan, imageSize, VK_BUFFER_USAGE_TRANSFER_SRC_BIT, VMA_MEMORY_USAGE_CPU_ONLY);
	if (pStagingBuffer == NULL)
	{
		return NULL;
	}


	/* Copy the image data to the staging buffer */
	void* pData = NULL;
	if (vmaMapMemory(pRenderDeviceVulkan->allocator, pStagingBuffer->allocation, &pData) != VK_SUCCESS)
	{
		FRenderDeviceVulkanDestroyBuffer(pRenderDeviceVulkan, &pStagingBuffer);
		FTextureVulkanDestroy(pRenderDevice, &pTexture);
		return NULL;
	}
	
		FMemoryCopy(pOptions->pImageData, pData, imageSize);
	
	vmaUnmapMemory(pRenderDeviceVulkan->allocator, pStagingBuffer->allocation);

	/* Create the vulkan image */
	VkImageCreateInfo imageCreateInfo = {0};
	imageCreateInfo.sType = VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO;
	imageCreateInfo.pNext = NULL;
	imageCreateInfo.flags = 0;
	imageCreateInfo.imageType = VK_IMAGE_TYPE_2D;
	imageCreateInfo.format = ETextureFormatToVkFormat(pOptions->format);
	imageCreateInfo.extent = (VkExtent3D){pOptions->width, pOptions->height, 1};
	imageCreateInfo.mipLevels = 1;
	imageCreateInfo.arrayLayers = 0;
	imageCreateInfo.samples = VK_SAMPLE_COUNT_1_BIT;
	imageCreateInfo.tiling = VK_IMAGE_TILING_OPTIMAL;
	imageCreateInfo.usage = (imageCreateInfo.format == VK_FORMAT_R8G8B8A8_SRGB) ? VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT : VK_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT;
	imageCreateInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;
	imageCreateInfo.queueFamilyIndexCount = 0;
	imageCreateInfo.pQueueFamilyIndices = NULL;
	imageCreateInfo.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;

	VmaAllocationCreateInfo allocationCreateInfo = {0};
	allocationCreateInfo.flags = 0;
	allocationCreateInfo.usage = VMA_MEMORY_USAGE_CPU_COPY;
	allocationCreateInfo.requiredFlags = 0;
	allocationCreateInfo.preferredFlags = 0;
	allocationCreateInfo.memoryTypeBits = 0;
	allocationCreateInfo.pool = VK_NULL_HANDLE;
	allocationCreateInfo.pUserData = NULL;
	allocationCreateInfo.priority = 1.0f;

	if (vmaCreateImage(pRenderDeviceVulkan->allocator, &imageCreateInfo, &allocationCreateInfo, &pTextureVulkan->image, &pTextureVulkan->allocation, NULL) != VK_SUCCESS)
	{
		FRenderDeviceVulkanDestroyBuffer(pRenderDeviceVulkan, &pStagingBuffer);
		FTextureVulkanDestroy(pRenderDevice, &pTexture);
		return NULL;
	}

	/* Copy the staging buffer to the image buffer */
	VkCommandBuffer temporaryCommandBuffer = FRenderDeviceVulkanBeginTemporaryCommandBuffer(pRenderDeviceVulkan);
	if (temporaryCommandBuffer == VK_NULL_HANDLE)
	{
		FRenderDeviceVulkanDestroyBuffer(pRenderDeviceVulkan, &pStagingBuffer);
		FTextureVulkanDestroy(pRenderDevice, &pTexture);
		return NULL;
	}

		/* Transition to a transfer recieve layout */ 
		VkImageSubresourceRange subresource = {0};
		subresource.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
		subresource.baseMipLevel = 0;
		subresource.levelCount = 1;
		subresource.baseArrayLayer = 0;
		subresource.layerCount = 1;
		
		VkImageMemoryBarrier barrier = {0};
		barrier.sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER;
		barrier.pNext = NULL;
		barrier.srcAccessMask = 0;
		barrier.dstAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;
		barrier.oldLayout = VK_IMAGE_LAYOUT_UNDEFINED;
		barrier.newLayout = VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL;
		barrier.srcQueueFamilyIndex = 0;
		barrier.dstQueueFamilyIndex = 0;
		barrier.image = pTextureVulkan->image;
		barrier.subresourceRange = subresource;

		vkCmdPipelineBarrier(temporaryCommandBuffer, VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT, VK_PIPELINE_STAGE_TRANSFER_BIT, 0, 0, NULL, 0, NULL, 1, &barrier);

		/* Copy the staging buffer to the images buffer */
		VkBufferImageCopy region = {0};
		region.bufferOffset = 0;
		region.bufferRowLength = 0;
		region.bufferImageHeight = 0;
		region.imageSubresource.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
		region.imageSubresource.mipLevel = 0;
		region.imageSubresource.baseArrayLayer = 0;
		region.imageSubresource.layerCount = 1;
		region.imageOffset = (VkOffset3D){0};
		region.imageExtent = (VkExtent3D){pOptions->width, pOptions->height, 1};
		
		vkCmdCopyBufferToImage(temporaryCommandBuffer, pStagingBuffer->buffer, pTextureVulkan->image, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, 1, &region);

	FRenderDeviceVulkanEndTemporaryCommandBuffer(pRenderDeviceVulkan, temporaryCommandBuffer);

	/* Destroy the staging buffer */
	FRenderDeviceVulkanDestroyBuffer(pRenderDeviceVulkan, &pStagingBuffer);

	return pTexture;
}

void FTextureVulkanDestroy(FRenderDevice* pRenderDevice, FTexture** ppTexture)
{
	if (pRenderDevice == NULL || ppTexture == NULL || *ppTexture == NULL)
	{
		return;
	}

	FRenderDeviceVulkan* pRenderDeviceVulkan = (FRenderDeviceVulkan*)pRenderDevice;
	FTextureVulkan* pTextureVulkan = (FTextureVulkan*)*ppTexture;

	if (pTextureVulkan->image != NULL && pTextureVulkan->allocation != NULL)
	{
		vmaDestroyImage(pRenderDeviceVulkan->allocator, pTextureVulkan->image, pTextureVulkan->allocation);
	}

	FDeallocate(pTextureVulkan);
	*ppTexture = NULL;
}

FMesh* FMeshVulkanCreate(FRenderDevice* pRenderDevice, const FMeshCreateOptions* pOptions)
{
    if (pRenderDevice == NULL || pOptions == NULL)
    {
        return NULL;
    }

	FRenderDeviceVulkan* pRenderDeviceVulkan = (FRenderDeviceVulkan*)pRenderDevice;

	FMeshVulkan* pMeshVulkan = FAllocateZero(1, sizeof(FMeshVulkan));
	if (pMeshVulkan == NULL)
	{
		return NULL;
	}

	FMesh* pMesh = (FMesh*)pMeshVulkan;

	/* Create the staging buffer */
	FInt indexOffset = (pOptions->verticesCount * sizeof(FVertex));
	FInt indexSize = (pOptions->indicesCount * sizeof(FInt32));
	FInt size = indexOffset + indexSize;
	
	FBufferVulkan* pStagingBuffer = FRenderDeviceVulkanCreateBuffer(pRenderDeviceVulkan, size, VK_BUFFER_USAGE_TRANSFER_SRC_BIT, VMA_MEMORY_USAGE_CPU_COPY);
	if (pStagingBuffer == NULL)
	{
		return NULL;
	}

	/* Copy the vertex data to the staging buffer */
	void* pData = NULL;
	if (vmaMapMemory(pRenderDeviceVulkan->allocator, pStagingBuffer->allocation, &pData) != VK_SUCCESS)
	{
		FRenderDeviceVulkanDestroyBuffer(pRenderDeviceVulkan, &pStagingBuffer);
		FMeshVulkanDestroy(pRenderDevice, &pMesh);
	}

		FMemoryCopy(pOptions->pVertices, pData, size);
		FMemoryCopy(pOptions->pIndex, (char*)pData + indexOffset, indexSize);

	vmaUnmapMemory(pRenderDeviceVulkan->allocator, pStagingBuffer->allocation);

	/* Create the vertex buffer */
	pMeshVulkan->pBuffer = FRenderDeviceVulkanCreateBuffer(pRenderDeviceVulkan, size, VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_VERTEX_BUFFER_BIT, VMA_MEMORY_USAGE_GPU_ONLY);
	if (pMeshVulkan->pBuffer)
	{
		FRenderDeviceVulkanDestroyBuffer(pRenderDeviceVulkan, &pStagingBuffer);
		FMeshVulkanDestroy(pRenderDevice, &pMesh);
	}

	/* Begin a command buffer to copy from the staging buffer to the vertex buffer */
	VkCommandBuffer temporaryCommandBuffer = FRenderDeviceVulkanBeginTemporaryCommandBuffer(pRenderDeviceVulkan);
	if (temporaryCommandBuffer == VK_NULL_HANDLE)
	{
		FRenderDeviceVulkanDestroyBuffer(pRenderDeviceVulkan, &pStagingBuffer);
		FMeshVulkanDestroy(pRenderDevice, &pMesh);
		return NULL;
	}

		/* Copy the whole buffer */
		VkBufferCopy region = {0};
		region.srcOffset = 0;
		region.dstOffset = 0;
		region.size = size;

		vkCmdCopyBuffer(temporaryCommandBuffer, pStagingBuffer->buffer, pMeshVulkan->pBuffer->buffer, 1, &region);

	FRenderDeviceVulkanEndTemporaryCommandBuffer(pRenderDeviceVulkan, temporaryCommandBuffer);

	/* Destroy the staging buffer */
	FRenderDeviceVulkanDestroyBuffer(pRenderDeviceVulkan, &pStagingBuffer);

	return pMesh;
}

void FMeshVulkanDestroy(FRenderDevice* pRenderDevice, FMesh** ppMesh)
{
	if (pRenderDevice == NULL || ppMesh == NULL || *ppMesh == NULL)
	{
		return;
	}

	FRenderDeviceVulkan* pRenderDeviceVulkan = (FRenderDeviceVulkan*)pRenderDevice;
	FMeshVulkan* pMeshVulkan = (FMeshVulkan*)*ppMesh;

	FRenderDeviceVulkanDestroyBuffer(pRenderDeviceVulkan, &pMeshVulkan->pBuffer);
}

FMaterial* FMaterialVulkanCreate(FRenderDevice* pRenderDevice, const FMaterialCreateOptions* pOptions)
{

}

void FMaterialVulkanDestroy(FRenderDevice* pRenderDevice, FMaterial** ppMaterial)
{

}

FMaterialInstance* FMaterialInstanceVulkanCreate(FRenderDevice* pRenderDevice, FMaterial* pMaterial)
{

}

void FMaterialInstanceVulkanDestroy(FRenderDevice* pRenderDevice, FMaterialInstance** ppMaterialInstance)
{

}

void FMaterialInstanceVulkanSetFloat(FRenderDevice* pRenderDevice, FMaterialInstance* pMaterialInstance, const char* pName, float value)
{

}

void FMaterialInstanceVulkanSetVector2(FRenderDevice* pRenderDevice, FMaterialInstance* pMaterialInstance, const char* pName, const FVector2* value)
{

}

void FMaterialInstanceVulkanSetVector3(FRenderDevice* pRenderDevice, FMaterialInstance* pMaterialInstance, const char* pName, const FVector3* value)
{

}

void FMaterialInstanceVulkanSetVector4(FRenderDevice* pRenderDevice, FMaterialInstance* pMaterialInstance, const char* pName, const FVector4* value)
{

}

void FMaterialInstanceVulkanSetMatrix4(FRenderDevice* pRenderDevice, FMaterialInstance* pMaterialInstance, const char* pName, const FMatrix4* value)
{

}

void FMaterialInstanceVulkanSetTexture(FRenderDevice* pRenderDevice, FMaterialInstance* pMaterialInstance, const char* pName, FTexture* pTexture)
{

}


void FRenderDeviceVulkanSurfaceResizeCallback(FRenderDevice* pRenderDevice, FInt width, FInt height)
{
	FRenderDeviceVulkan* pRenderDeviceVulkan = (FRenderDeviceVulkan*)pRenderDevice;


	VkExtent2D extent = {(uint32_t)width, (uint32_t)height};
	FRenderDeviceVulkanDestroySwapchain(pRenderDeviceVulkan);
	FRenderDeviceVulkanCreateSwapchain(pRenderDeviceVulkan, extent);
}



/* 	================================================================================
	================================================================================
	================================================================================
	================================================================================  */



const char* const* FRenderDeviceVulkanGetExtensions(uint32_t* pCount)
{
	/* Check the parameters */
	if (pCount == NULL)
	{
		return NULL;
	}

	(*pCount) = 0;

#ifdef ENTERPRISE_DEBUG
	const uint32_t requiredInstanceExtensionsCount = 3;
	static const char* ppRequiredInstanceExtensions[3] = 
	{
		VK_KHR_SURFACE_EXTENSION_NAME,   
#ifdef ENTERPRISE_WINDOWS
		"VK_KHR_win32_surface",
#elif ENTERPRISE_LINUX
		"VK_KHR_xcb_surface",
#endif
		VK_EXT_DEBUG_UTILS_EXTENSION_NAME
	};
#else
	const uint32_t requiredInstanceExtensionsCount = 2;
	static const char* ppRequiredInstanceExtensions[2] = 
	{
		VK_KHR_SURFACE_EXTENSION_NAME,
#ifdef ENTERPRISE_WINDOWS
		"VK_KHR_win32_surface",
#elif ENTERPRISE_LINUX
		"VK_KHR_xcb_surface",
#endif
	};
#endif

	/* Enumerate the instance extensions and create an array to hold the data */
	uint32_t foundExtensionsCount = 0;
	if (vkEnumerateInstanceExtensionProperties(NULL, &foundExtensionsCount, NULL) != VK_SUCCESS || foundExtensionsCount == 0)
	{
		return NULL;
	}
	
	VkExtensionProperties* pProperties = calloc(foundExtensionsCount, sizeof(VkExtensionProperties));
	if (pProperties == NULL)
	{
		return NULL;
	}

	if (vkEnumerateInstanceExtensionProperties(NULL, &foundExtensionsCount, pProperties) != VK_SUCCESS)
	{
		FDeallocate(pProperties);
		return NULL;
	}

	/* Enumerate over the instance extensions and find if there aren't any matches */
	for (uint32_t i = 0; i < requiredInstanceExtensionsCount; i++)
	{
		bool bFound = false;
		for (uint32_t j = 0; j < foundExtensionsCount; j++)
		{
			if (FStringCompare(ppRequiredInstanceExtensions[i], sizeof(ppRequiredInstanceExtensions[i]), pProperties[j].extensionName, VK_MAX_EXTENSION_NAME_SIZE) == 0)
			{
				bFound = true;
				break;
			}
		}

		if (!bFound)
		{
			FDeallocate(pProperties);
			return NULL;
		}
	}

	/* We found all of the required extensions return with them */
	FDeallocate(pProperties);
	(*pCount) = requiredInstanceExtensionsCount;
	return (const char**)ppRequiredInstanceExtensions;
}

const char* const* FRenderDeviceVulkanGetLayers(uint32_t* pCount)
{
	/* Check the parameters */
	if (pCount == NULL)
	{
		return NULL;
	}

	(*pCount) = 0;

	const uint32_t requiredLayersCount = 1;
	static const char* ppRequiredLayers[1] =
	{
		"VK_LAYER_KHRONOS_validation"
	};

	/* Enumerate the instance layers and create an array to hold the data */
	uint32_t foundLayersCount = 0;
	if (vkEnumerateInstanceLayerProperties(&foundLayersCount, NULL) != VK_SUCCESS || foundLayersCount == 0)
	{
		return NULL;
	}

	VkLayerProperties* pProperties = FAllocateZero(foundLayersCount, sizeof(VkLayerProperties));
	if (pProperties == NULL)
	{
		return NULL;
	}

	if (vkEnumerateInstanceLayerProperties(&foundLayersCount, pProperties) != VK_SUCCESS)
	{
		free(pProperties);
		return NULL;
	}

	/* Enumerate over the instance layers and find if there aren't any matches */
	for (uint32_t i = 0; i < requiredLayersCount; i++)
	{
		bool bFound = false;
		for (uint32_t j = 0; j < foundLayersCount; j++)
		{
			if (FStringCompare(ppRequiredLayers[i], sizeof(ppRequiredLayers[i]), pProperties[j].layerName, VK_MAX_EXTENSION_NAME_SIZE) == 0)
			{
				bFound = true;
				break;
			}
		}

		if (!bFound)
		{
			FDeallocate(pProperties);
			return NULL;
		}
	}

	/* We found all of the required layers return with them */
	FDeallocate(pProperties);
	(*pCount) = requiredLayersCount;
	return ppRequiredLayers;
}

const char* const* FRenderDeviceVulkanGetDeviceExtensions(FRenderDeviceVulkan* pRenderData, uint32_t* pCount)
{
	if (pCount == NULL)
	{
		return NULL;
	}

	*pCount = 0;

	const uint32_t requiredDeviceExtensionsCount = 1;
	static const char* ppRequiredDeviceExtensions[1] = 
	{ 
		VK_KHR_SWAPCHAIN_EXTENSION_NAME 
	};

	/* Get the device extensions */
	uint32_t foundExtensionCount = 0;
	if (vkEnumerateDeviceExtensionProperties(pRenderData->physicalDevice, NULL, &foundExtensionCount, NULL) != VK_SUCCESS || foundExtensionCount == 0)
	{
		return NULL;
	}

	VkExtensionProperties* pFoundExtensions = FAllocateZero(1, sizeof(VkExtensionProperties) * foundExtensionCount);
	if (pFoundExtensions == NULL)
	{
		return NULL;
	}

	if (vkEnumerateDeviceExtensionProperties(pRenderData->physicalDevice, NULL, &foundExtensionCount, pFoundExtensions) != VK_SUCCESS)
	{
		FDeallocate(pFoundExtensions);
		return NULL;
	}

	/* Check if all required extensions are available */
	for (uint32_t i = 0; i < requiredDeviceExtensionsCount; i++)
	{
		bool bFound = false;
		for (uint32_t j = 0; j < foundExtensionCount; j++)
		{
			if (FStringCompare(ppRequiredDeviceExtensions[i], sizeof(ppRequiredDeviceExtensions[i]), pFoundExtensions[j].extensionName, VK_MAX_EXTENSION_NAME_SIZE) == 0)
			{
				bFound = true;
				break;
			}
		}

		if (!bFound)
		{
			FDeallocate(pFoundExtensions);
			return NULL;
		}
	}

	FDeallocate(pFoundExtensions);
	*pCount = requiredDeviceExtensionsCount;
	return ppRequiredDeviceExtensions;
}


bool FRenderDeviceVulkanCreateInstance(FRenderDeviceVulkan* pRenderDevice, bool bDebug)
{

	/* Create the application info */
	VkApplicationInfo applicationInfo = {0};
	applicationInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
	applicationInfo.pNext = NULL;
	applicationInfo.pApplicationName = "Enterprise";
	applicationInfo.applicationVersion = VK_MAKE_VERSION(1, 0, 0);
	applicationInfo.pEngineName = "Enterprise";
	applicationInfo.engineVersion = VK_MAKE_VERSION(1, 0, 0);
	applicationInfo.apiVersion = VK_API_VERSION_1_2;

	/* Get the validation layers and instance extensions */
	uint32_t validationLayersCount = 0;
	const char* const* ppValidationLayers = FRenderDeviceVulkanGetLayers(&validationLayersCount);
	uint32_t enabledExtensionsCount = 0;
	const char* const* ppEnabledExtensions = FRenderDeviceVulkanGetExtensions(&enabledExtensionsCount);

#ifdef FDEBUG
	/* Create the debug utils info */
	VkDebugUtilsMessengerCreateInfoEXT debugUtilsMessengerCreateInfo = {0};
	debugUtilsMessengerCreateInfo.sType = VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT;
	debugUtilsMessengerCreateInfo.pNext = NULL;
	debugUtilsMessengerCreateInfo.flags = 0;
	debugUtilsMessengerCreateInfo.messageSeverity = VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT;
	debugUtilsMessengerCreateInfo.messageType = VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT;
	debugUtilsMessengerCreateInfo.pfnUserCallback = FVulkanDebugCallback;
	debugUtilsMessengerCreateInfo.pUserData = NULL;
#endif

	/* Create the instance */
	VkInstanceCreateInfo instanceCreateInfo = {0};
	instanceCreateInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
#ifdef FDEBUG
	instanceCreateInfo.pNext = &debugUtilsMessengerCreateInfo;
#else
	instanceCreateInfo.pNext = NULL;
#endif
	instanceCreateInfo.flags = 0;
	instanceCreateInfo.pApplicationInfo = &applicationInfo;
	instanceCreateInfo.enabledLayerCount = bDebug ? validationLayersCount : 0;
	instanceCreateInfo.ppEnabledLayerNames = bDebug ? ppValidationLayers : NULL;
	instanceCreateInfo.enabledExtensionCount = enabledExtensionsCount;
	instanceCreateInfo.ppEnabledExtensionNames = ppEnabledExtensions;

	if (vkCreateInstance(&instanceCreateInfo, NULL, &pRenderDevice->instance) != VK_SUCCESS)
	{
		return false;
	}

#ifdef FDEBUG
	/* Load the debug messenger functions */
	pRenderDevice->CreateDebugUtilsMessenger = (PFN_vkCreateDebugUtilsMessengerEXT)vkGetInstanceProcAddr(pRenderDevice->instance, "vkCreateDebugUtilsMessengerEXT");
	pRenderDevice->DestroyDebugUtilsMessenger = (PFN_vkDestroyDebugUtilsMessengerEXT)vkGetInstanceProcAddr(pRenderDevice->instance, "vkDestroyDebugUtilsMessengerEXT");

	if (pRenderDevice->CreateDebugUtilsMessenger == NULL || pRenderDevice->DestroyDebugUtilsMessenger == NULL)
	{
		return false;
	}

	/* Create the debug messenger */
	if (pRenderDevice->CreateDebugUtilsMessenger(pRenderDevice->instance, &debugUtilsMessengerCreateInfo, NULL, &pRenderDevice->debugMessenger) != VK_SUCCESS)
	{
		return false;
	}
#endif

	return true;
}

void FRenderDeviceVulkanDestroyInstance(FRenderDeviceVulkan* pContext)
{
	if (pContext == NULL)
	{
		return;
	}

	if (pContext->DestroyDebugUtilsMessenger != NULL && pContext->debugMessenger != VK_NULL_HANDLE)
	{
		pContext->DestroyDebugUtilsMessenger(pContext->instance, pContext->debugMessenger, NULL);
	}

	if (pContext->instance != VK_NULL_HANDLE)
	{
		vkDestroyInstance(pContext->instance, NULL);
	}
}

VkBool32 FRenderDeviceVulkanDebugCallback(VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity, VkDebugUtilsMessageTypeFlagsEXT messageTypes, const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData, void* pUserData)
{
	switch (messageSeverity)
	{
	case VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT:
	case VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT:
		FLogError(pCallbackData->pMessage);
		break;
	
	case VK_DEBUG_UTILS_MESSAGE_SEVERITY_INFO_BIT_EXT:
		FLogInfo(pCallbackData->pMessage);
		break;

	default:
		break;
	}

	return VK_FALSE;
}

bool FRenderDeviceVulkanCreateSurface(FRenderer* pRenderer, FRenderDeviceVulkan* pRenderDevice)
{
	if (pRenderer == NULL || pRenderDevice == NULL)
	{
		return false;
	}

	if (SDL_Vulkan_CreateSurface(pRenderer->pWindow, pRenderDevice->instance, &pRenderDevice->surface) != SDL_TRUE)
	{
		return false;
	}

	return true;
}

void FRenderDeviceVulkanDestroySurface(FRenderDeviceVulkan* pRenderData)
{
	if (pRenderData == NULL)
	{
		return;
	}

	if (pRenderData->surface != VK_NULL_HANDLE)
	{
		vkDestroySurfaceKHR(pRenderData->instance, pRenderData->surface, NULL);
	}
}

bool FRenderDeviceVulkanChoosePhysicalDevice(FRenderDeviceVulkan* pRenderDevice, FInt id)
{
	if (pRenderDevice == NULL)
	{
		return false;
	}

	/* Create the physical devices array */
	uint32_t physicalDeviceCount = 0;
	if (vkEnumeratePhysicalDevices(pRenderDevice->instance, &physicalDeviceCount, NULL) != VK_SUCCESS || physicalDeviceCount == 0)
	{
		return false;
	}

	if (physicalDeviceCount == 0)
	{
		FLogError("No graphics devices found!");
		return false;
	}

	VkPhysicalDevice* pPhysicalDevices = FAllocateZero(1, sizeof(VkPhysicalDevice) * physicalDeviceCount);
	if (pPhysicalDevices == NULL)
	{
		return false;
	}

	if (vkEnumeratePhysicalDevices(pRenderDevice->instance, &physicalDeviceCount, pPhysicalDevices) != VK_SUCCESS)
	{
		FDeallocate(pPhysicalDevices);
		return false;
	}

	if (id < 0 || id >= (FInt)physicalDeviceCount)
	{
		FLogWarning("Specified physical device index was invalid! Using default zero.");       
	}

	/* Loop over all of the physical devices finding the requested one. */
	/* If the requested one wasnt found, use the last physical device. */
	VkPhysicalDevice physicalDevice = VK_NULL_HANDLE;
	for (FInt i = 0; i < (FInt)physicalDeviceCount; i++)
	{
		physicalDevice = pPhysicalDevices[i];
		
		VkPhysicalDeviceProperties properties = {0};
		vkGetPhysicalDeviceProperties(physicalDevice, &properties);

		if ((FInt)properties.deviceID == id)
		{
			break;
		}
	}

	pRenderDevice->physicalDevice = physicalDevice;
	FDeallocate(pPhysicalDevices);

	return true;
}

bool FRenderDeviceVulkanChooseFormats(FRenderDeviceVulkan* pRenderDevice)
{
	/* Get the surface format */
	uint32_t surfaceFormatsCount = 0;
	if (vkGetPhysicalDeviceSurfaceFormatsKHR(pRenderDevice->physicalDevice, pRenderDevice->surface, &surfaceFormatsCount, NULL) != VK_SUCCESS || surfaceFormatsCount == 0)
	{
		return false;
	}

	VkSurfaceFormatKHR* pSurfaceFormats = FAllocateZero(surfaceFormatsCount, sizeof(VkSurfaceFormatKHR));
	if (pSurfaceFormats == NULL)
	{
		return false;
	}

	if (vkGetPhysicalDeviceSurfaceFormatsKHR(pRenderDevice->physicalDevice, pRenderDevice->surface, &surfaceFormatsCount, pSurfaceFormats) != VK_SUCCESS)
	{
		FDeallocate(pSurfaceFormats);
		return false;
	}

	/* Loop over all of the surface formats and find one that fits the defaults */
	/* If none fit the default then use the first one */
	VkSurfaceFormatKHR surfaceFormat = {0};
	for (FInt i = surfaceFormatsCount; i > 0; i--)
	{
		surfaceFormat = pSurfaceFormats[i];

		/* Use VK_FORMAT_R8G8B8A8_SRGB and VK_COLOR_SPACE_SRGB_NONLINEAR_KHR as defaults */
		if (pSurfaceFormats[i].format == VK_FORMAT_R8G8B8A8_SRGB && pSurfaceFormats[i].colorSpace == VK_COLOR_SPACE_SRGB_NONLINEAR_KHR)
		{
			break;
		}
	}
	pRenderDevice->surfaceFormat = surfaceFormat;

	FDeallocate(pSurfaceFormats);

	/* ========== Get the present mode ========== */
	uint32_t presentModesCount = 0;
	if (vkGetPhysicalDeviceSurfacePresentModesKHR(pRenderDevice->physicalDevice, pRenderDevice->surface, &presentModesCount, NULL) != VK_SUCCESS || presentModesCount == 0)
	{
		return false;
	}

	VkPresentModeKHR* pPresentModes = FAllocateZero(presentModesCount, sizeof(VkPresentModeKHR));
	if (pPresentModes == NULL)
	{
		return false;
	}

	if (vkGetPhysicalDeviceSurfacePresentModesKHR(pRenderDevice->physicalDevice, pRenderDevice->surface, &presentModesCount, pPresentModes) != VK_SUCCESS)
	{
		FDeallocate(pPresentModes);
		return false;
	}

	/* We use MAILBOX because it is ususally the best but if not go with FIFO */
	VkPresentModeKHR presentMode = VK_PRESENT_MODE_FIFO_KHR;
	for (uint32_t i = 0; i < presentModesCount; i++)
	{
		if (pPresentModes[i] == VK_PRESENT_MODE_MAILBOX_KHR)
		{
			presentMode = VK_PRESENT_MODE_MAILBOX_KHR;
			break;
		}
	}

	pRenderDevice->presentMode = presentMode;

	FDeallocate(pPresentModes);
	return true;
}

bool FRenderDeviceVulkanChooseQueueFamilies(FRenderDeviceVulkan* pRenderDeviceVulkan)
{
	if (pRenderDeviceVulkan == NULL)
	{
		return false;
	}

	/* Get all of the device queues */
	uint32_t queueFamilyPropertyCount = 0;
	vkGetPhysicalDeviceQueueFamilyProperties(pRenderDeviceVulkan->physicalDevice, &queueFamilyPropertyCount, NULL);
	if (queueFamilyPropertyCount == 0)
	{
		return false;
	}

	VkQueueFamilyProperties* pQueueFamilyProperties = FAllocateZero(1, sizeof(VkQueueFamilyProperties) * queueFamilyPropertyCount);
	if (pQueueFamilyProperties == NULL)
	{
		return false;
	}

	vkGetPhysicalDeviceQueueFamilyProperties(pRenderDeviceVulkan->physicalDevice, &queueFamilyPropertyCount, pQueueFamilyProperties);
	
	/* Enumerate over all of the queue family properties */
	pRenderDeviceVulkan->graphicsFamilyIndex = -1;
	pRenderDeviceVulkan->presentFamilyIndex = -1;
	pRenderDeviceVulkan->computeFamilyIndex = -1;

	for (FInt i = 0; i < (FInt)queueFamilyPropertyCount; i++)
	{
		if (pQueueFamilyProperties[i].queueFlags & VK_QUEUE_GRAPHICS_BIT)
		{
			pRenderDeviceVulkan->graphicsFamilyIndex = (uint32_t)i;
		}

		VkBool32 canPresent = VK_FALSE;
		vkGetPhysicalDeviceSurfaceSupportKHR(pRenderDeviceVulkan->physicalDevice, i, pRenderDeviceVulkan->surface, &canPresent);
		if (canPresent)
		{
			pRenderDeviceVulkan->presentFamilyIndex = (uint32_t)i;
		}

		/* Find compute queue without graphics capabilities */
		if (pQueueFamilyProperties[i].queueFlags & VK_QUEUE_COMPUTE_BIT && !(pQueueFamilyProperties[i].queueFlags & VK_QUEUE_GRAPHICS_BIT))
		{
			pRenderDeviceVulkan->computeFamilyIndex = i;
			pRenderDeviceVulkan->bHasCompute = true;
		}
	}

	FDeallocate(pQueueFamilyProperties);
	return true;
}

bool FRenderDeviceVulkanCreateDevice(FRenderDeviceVulkan* pRenderDevice, bool bDebug)
{
	if (pRenderDevice == NULL || pRenderDevice->instance == VK_NULL_HANDLE || pRenderDevice->physicalDevice == VK_NULL_HANDLE)
	{
		return false;
	}

	bool graphicsAndPresentSame = pRenderDevice->graphicsFamilyIndex == pRenderDevice->presentFamilyIndex;
	bool computeAndPresentSame = pRenderDevice->computeFamilyIndex == pRenderDevice->presentFamilyIndex;

	/* Create the device */
	float pQueuePriorities[3] = {1.0f};
	VkDeviceQueueCreateInfo pDeviceQueueCreateInfos[3] = {0};
	uint32_t deviceQueueCreateInfosCount = 1;

	pDeviceQueueCreateInfos[0].sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
	pDeviceQueueCreateInfos[0].pNext = NULL;
	pDeviceQueueCreateInfos[0].flags = 0;
	pDeviceQueueCreateInfos[0].queueFamilyIndex = pRenderDevice->graphicsFamilyIndex;
	pDeviceQueueCreateInfos[0].queueCount = 1;
	pDeviceQueueCreateInfos[0].pQueuePriorities = pQueuePriorities;

	if (!graphicsAndPresentSame)
	{
		deviceQueueCreateInfosCount++;
		pDeviceQueueCreateInfos[1].sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
		pDeviceQueueCreateInfos[1].pNext = NULL;
		pDeviceQueueCreateInfos[1].flags = 0;
		pDeviceQueueCreateInfos[1].queueFamilyIndex = pRenderDevice->presentFamilyIndex;
		pDeviceQueueCreateInfos[1].queueCount = 1;
		pDeviceQueueCreateInfos[1].pQueuePriorities = pQueuePriorities;
	}

	if (!computeAndPresentSame && pRenderDevice->bHasCompute)
	{
		deviceQueueCreateInfosCount++;
		pDeviceQueueCreateInfos[2].sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
		pDeviceQueueCreateInfos[2].pNext = NULL;
		pDeviceQueueCreateInfos[2].flags = 0;
		pDeviceQueueCreateInfos[2].queueFamilyIndex = pRenderDevice->computeFamilyIndex;
		pDeviceQueueCreateInfos[2].queueCount = 1;
		pDeviceQueueCreateInfos[2].pQueuePriorities = pQueuePriorities;
	}

	/* Get the layers and extensions */
	uint32_t enabledValidationLayerCount = 0;
	const char* const* ppEnabledValidationLayers = FRenderDeviceVulkanGetLayers(&enabledValidationLayerCount);
	uint32_t enabledDeviceExtensionsCount = 0;
	const char* const* ppEnabledDeviceExtensions = FRenderDeviceVulkanGetDeviceExtensions(pRenderDevice, &enabledDeviceExtensionsCount);

	VkPhysicalDeviceFeatures deviceFeatures = {0};

	VkDeviceCreateInfo deviceCreateInfo = {0};
	deviceCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
	deviceCreateInfo.pNext = NULL;
	deviceCreateInfo.flags = 0;
	deviceCreateInfo.queueCreateInfoCount = deviceQueueCreateInfosCount;
	deviceCreateInfo.pQueueCreateInfos = pDeviceQueueCreateInfos;
	deviceCreateInfo.enabledLayerCount = bDebug ? enabledValidationLayerCount : 0;
	deviceCreateInfo.ppEnabledLayerNames = bDebug ? ppEnabledValidationLayers : NULL;
	deviceCreateInfo.enabledExtensionCount = enabledDeviceExtensionsCount;
	deviceCreateInfo.ppEnabledExtensionNames = ppEnabledDeviceExtensions;
	deviceCreateInfo.pEnabledFeatures = &deviceFeatures;

	if (vkCreateDevice(pRenderDevice->physicalDevice, &deviceCreateInfo, NULL, &pRenderDevice->device) != VK_SUCCESS)
	{
		return false;
	}

	/* Get the device queues */
	vkGetDeviceQueue(pRenderDevice->device, pRenderDevice->graphicsFamilyIndex, 0, &pRenderDevice->graphicsQueue);
	vkGetDeviceQueue(pRenderDevice->device, pRenderDevice->presentFamilyIndex, 0, &pRenderDevice->presentQueue);

	if (pRenderDevice->bHasCompute)
	{
		vkGetDeviceQueue(pRenderDevice->device, pRenderDevice->computeFamilyIndex, 0, &pRenderDevice->computeQueue);
	}

	return true;
}

void FRenderDeviceVulkanDestroyDevice(FRenderDeviceVulkan* pRenderDevice)
{
	if (pRenderDevice == NULL)
	{
		return;
	}

	vkDeviceWaitIdle(pRenderDevice->device);

	if (pRenderDevice->device != VK_NULL_HANDLE)
	{
		vkDestroyDevice(pRenderDevice->device, NULL);
	}
}

bool FRenderDeviceVulkanCreateAllocator(FRenderDeviceVulkan* pRenderDevice)
{
	if (pRenderDevice == NULL)
	{
		return false;
	}

	VmaAllocatorCreateInfo allocatorCreateInfo = {0};
	allocatorCreateInfo.flags = 0;
	allocatorCreateInfo.physicalDevice = pRenderDevice->physicalDevice;
	allocatorCreateInfo.device = pRenderDevice->device;
	allocatorCreateInfo.preferredLargeHeapBlockSize = 0;
	allocatorCreateInfo.pAllocationCallbacks = NULL;
	allocatorCreateInfo.pDeviceMemoryCallbacks = NULL;
	allocatorCreateInfo.frameInUseCount = 0;
	allocatorCreateInfo.pHeapSizeLimit = 0;
	allocatorCreateInfo.pVulkanFunctions = VK_NULL_HANDLE;
	allocatorCreateInfo.pRecordSettings = NULL;
	allocatorCreateInfo.instance = pRenderDevice->instance;
	allocatorCreateInfo.vulkanApiVersion = VK_API_VERSION_1_2;

	if (vmaCreateAllocator(&allocatorCreateInfo, &pRenderDevice->allocator) != VK_SUCCESS)
	{
		return false;
	}

	return true;
}

void FRenderDeviceVulkanDestroyAllocator(FRenderDeviceVulkan* pRenderDeviceVulkan)
{

	if (pRenderDeviceVulkan == NULL)
	{
		return;
	}

	vmaDestroyAllocator(pRenderDeviceVulkan->allocator);
}

bool FRenderDeviceVulkanCreateRenderpasses(FRenderDeviceVulkan* pRenderDevice)
{
	if (pRenderDevice == NULL)
	{
		return false;
	}

	VkAttachmentDescription attachmentDescription = {0};
	attachmentDescription.format = pRenderDevice->surfaceFormat.format;
	attachmentDescription.samples = VK_SAMPLE_COUNT_1_BIT;
	attachmentDescription.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
	attachmentDescription.storeOp = VK_ATTACHMENT_STORE_OP_STORE;
	attachmentDescription.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
	attachmentDescription.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
	attachmentDescription.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
	attachmentDescription.finalLayout = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR;

	VkAttachmentReference attachmentReference = {0};
	attachmentReference.attachment = 0;
	attachmentReference.layout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;

	VkSubpassDescription subpassDescription = {0};
	subpassDescription.pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS;
	subpassDescription.colorAttachmentCount = 1;
	subpassDescription.pColorAttachments = &attachmentReference;

	VkSubpassDependency subpassDependency = {0};
	subpassDependency.srcSubpass = VK_SUBPASS_EXTERNAL;
	subpassDependency.dstSubpass = 0;
	subpassDependency.srcStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
	subpassDependency.dstStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
	subpassDependency.srcAccessMask = 0;
	subpassDependency.dstAccessMask = VK_ACCESS_COLOR_ATTACHMENT_READ_BIT | VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;
	subpassDependency.dependencyFlags = 0;

	VkRenderPassCreateInfo renderPassCreateInfo = {0};
	renderPassCreateInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO;
	renderPassCreateInfo.pNext = NULL;
	renderPassCreateInfo.flags = 0;
	renderPassCreateInfo.attachmentCount = 1;
	renderPassCreateInfo.pAttachments = &attachmentDescription;
	renderPassCreateInfo.subpassCount = 1;
	renderPassCreateInfo.pSubpasses = &subpassDescription;
	renderPassCreateInfo.dependencyCount = 1;
	renderPassCreateInfo.pDependencies = &subpassDependency;

	if (vkCreateRenderPass(pRenderDevice->device, &renderPassCreateInfo, NULL, &pRenderDevice->renderPass) != VK_SUCCESS)
	{
		return false;
	}

	return true;
}

void FRenderDeviceVulkanDestroyRenderpasses(FRenderDeviceVulkan* pRenderDevice)
{
	if (pRenderDevice == NULL)
	{
		return;
	}

	if (pRenderDevice->renderPass != VK_NULL_HANDLE)
	{
		vkDestroyRenderPass(pRenderDevice->device, pRenderDevice->renderPass, NULL);
	}
}

bool FRenderDeviceVulkanCreateSwapchain(FRenderDeviceVulkan* pRenderDevice, VkExtent2D extent)
{
	if (pRenderDevice == NULL)
	{
		return false;
	}

	VkSurfaceCapabilitiesKHR surfaceCapabilities;
	if (vkGetPhysicalDeviceSurfaceCapabilitiesKHR(pRenderDevice->physicalDevice, pRenderDevice->surface, &surfaceCapabilities) != VK_SUCCESS)
	{
		return false;
	}

	/* Set the image account to minCount + 1, if that is too large just use the min */
	pRenderDevice->imageCount = ((surfaceCapabilities.minImageCount + 1) > surfaceCapabilities.maxImageCount) ? surfaceCapabilities.maxImageCount : surfaceCapabilities.minImageCount + 1;

	/* Get the queue image sharing data */
	bool graphicsAndPresentSame = pRenderDevice->graphicsFamilyIndex == pRenderDevice->presentFamilyIndex;
	uint32_t pQueueFamilyIndices[2] =
	{
		pRenderDevice->graphicsFamilyIndex,
		pRenderDevice->presentFamilyIndex
	};

	/* Create the swapchain */
	VkSwapchainCreateInfoKHR swapChainCreateInfo = {0};
	swapChainCreateInfo.sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
	swapChainCreateInfo.pNext = NULL;
	swapChainCreateInfo.flags = 0;
	swapChainCreateInfo.surface = pRenderDevice->surface;
	swapChainCreateInfo.minImageCount = pRenderDevice->imageCount;
	swapChainCreateInfo.imageFormat = pRenderDevice->surfaceFormat.format;
	swapChainCreateInfo.imageColorSpace = pRenderDevice->surfaceFormat.colorSpace;
	swapChainCreateInfo.imageExtent = extent;
	swapChainCreateInfo.imageArrayLayers = 1;
	swapChainCreateInfo.imageUsage = VK_IMAGE_USAGE_TRANSFER_DST_BIT | VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;
	swapChainCreateInfo.imageSharingMode = graphicsAndPresentSame ? VK_SHARING_MODE_EXCLUSIVE : VK_SHARING_MODE_CONCURRENT;
	swapChainCreateInfo.queueFamilyIndexCount =  graphicsAndPresentSame ? 0 : 2;
	swapChainCreateInfo.pQueueFamilyIndices = graphicsAndPresentSame ? NULL : pQueueFamilyIndices;
	swapChainCreateInfo.preTransform = surfaceCapabilities.currentTransform;
	swapChainCreateInfo.compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;
	swapChainCreateInfo.presentMode = pRenderDevice->presentMode;
	swapChainCreateInfo.clipped = VK_TRUE;
	swapChainCreateInfo.oldSwapchain = VK_NULL_HANDLE;

	if (vkCreateSwapchainKHR(pRenderDevice->device, &swapChainCreateInfo, NULL, &pRenderDevice->swapchain) != VK_SUCCESS)
	{
		return false;
	}

	/* Acquire the swapchain images */
	if (vkGetSwapchainImagesKHR(pRenderDevice->device, pRenderDevice->swapchain, &pRenderDevice->imageCount, NULL) != VK_SUCCESS)
	{
		FRenderDeviceVulkanDestroySwapchain(pRenderDevice);
		return false;
	}

	pRenderDevice->pSwapchainImages = FAllocateZero(pRenderDevice->imageCount, sizeof(VkImage));
	if (pRenderDevice->pSwapchainImages == NULL)
	{
		FRenderDeviceVulkanDestroySwapchain(pRenderDevice);
		return false;
	}

	if (vkGetSwapchainImagesKHR(pRenderDevice->device, pRenderDevice->swapchain, &pRenderDevice->imageCount, pRenderDevice->pSwapchainImages) != VK_SUCCESS)
	{
		FRenderDeviceVulkanDestroySwapchain(pRenderDevice);
		return false;
	}

	/* Create the swapchain images views */
	pRenderDevice->pSwapchainImageViews = FAllocateZero(pRenderDevice->imageCount, sizeof(VkImageView));
	pRenderDevice->pSwapchainFramebuffers = FAllocateZero(pRenderDevice->imageCount, sizeof(VkFramebuffer));

	if (pRenderDevice->pSwapchainImageViews == NULL || pRenderDevice->pSwapchainFramebuffers == NULL)
	{   
		FRenderDeviceVulkanDestroySwapchain(pRenderDevice);
		return false;
	}

	for (uint32_t i = 0; i < pRenderDevice->imageCount; i++)
	{
		/* Create the image view */
		VkImageViewCreateInfo imageViewCreateInfo = {0};
		imageViewCreateInfo.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
		imageViewCreateInfo.pNext = NULL;
		imageViewCreateInfo.flags = 0;
		imageViewCreateInfo.image = pRenderDevice->pSwapchainImages[i];
		imageViewCreateInfo.viewType = VK_IMAGE_VIEW_TYPE_2D;
		imageViewCreateInfo.format = pRenderDevice->surfaceFormat.format;
		imageViewCreateInfo.components = (VkComponentMapping){VK_COMPONENT_SWIZZLE_IDENTITY, VK_COMPONENT_SWIZZLE_IDENTITY, VK_COMPONENT_SWIZZLE_IDENTITY, VK_COMPONENT_SWIZZLE_IDENTITY};
		imageViewCreateInfo.subresourceRange = (VkImageSubresourceRange){VK_IMAGE_ASPECT_COLOR_BIT, 0, 1, 0, 1};

		if (vkCreateImageView(pRenderDevice->device, &imageViewCreateInfo, NULL, &pRenderDevice->pSwapchainImageViews[i]) != VK_SUCCESS)
		{
			FRenderDeviceVulkanDestroySwapchain(pRenderDevice);
			return false;
		}

		/* Create the framebuffer */
		VkFramebufferCreateInfo framebufferCreateInfo = {0};
		framebufferCreateInfo.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
		framebufferCreateInfo.pNext = NULL;
		framebufferCreateInfo.flags = 0;
		framebufferCreateInfo.renderPass = pRenderDevice->renderPass;
		framebufferCreateInfo.attachmentCount = 1;
		framebufferCreateInfo.pAttachments = &pRenderDevice->pSwapchainImageViews[i];
		framebufferCreateInfo.width = extent.width;
		framebufferCreateInfo.height = extent.height;
		framebufferCreateInfo.layers = 1;

		if (vkCreateFramebuffer(pRenderDevice->device, &framebufferCreateInfo, NULL, &pRenderDevice->pSwapchainFramebuffers[i]) != VK_SUCCESS)
		{
			FRenderDeviceVulkanDestroySwapchain(pRenderDevice);
			return false;
		}
	}

	return true;
}

void FRenderDeviceVulkanDestroySwapchain(FRenderDeviceVulkan* pRenderDevice)
{
	if (pRenderDevice == NULL)
	{
		return;
	}

	/* Destroy the framebuffers */
	if (pRenderDevice->pSwapchainFramebuffers != NULL)
	{
		for (uint32_t i = 0; i < pRenderDevice->imageCount; i++)
		{
			if (pRenderDevice->pSwapchainFramebuffers[i] != VK_NULL_HANDLE)
			{
				vkDestroyFramebuffer(pRenderDevice->device, pRenderDevice->pSwapchainFramebuffers[i], NULL);
			}
		}
		free(pRenderDevice->pSwapchainFramebuffers);
		pRenderDevice->pSwapchainFramebuffers = NULL;
	}

	/* Destroy the image views */
	if (pRenderDevice->pSwapchainImageViews != NULL)
	{
		for (uint32_t i = 0; i < pRenderDevice->imageCount; i++)
		{
			if (pRenderDevice->pSwapchainImageViews[i] != VK_NULL_HANDLE)
			{
				vkDestroyImageView(pRenderDevice->device, pRenderDevice->pSwapchainImageViews[i], NULL);
			}
		}
		FDeallocate(pRenderDevice->pSwapchainImageViews);
		pRenderDevice->pSwapchainImageViews = NULL;
	}

	/* Destroy the swapchain */
	if (pRenderDevice->pSwapchainImages != NULL)
	{
		FDeallocate(pRenderDevice->pSwapchainImages);
	}

	if (pRenderDevice->swapchain != VK_NULL_HANDLE)
	{
		vkDestroySwapchainKHR(pRenderDevice->device, pRenderDevice->swapchain, NULL);
	}
}

bool FRenderDeviceVulkanCreatePools(FRenderDeviceVulkan* pRenderDevice)
{
	if (pRenderDevice == NULL)
	{
		return false;
	}

	VkCommandPoolCreateInfo commandPoolCreateInfo = {0};
	commandPoolCreateInfo.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
	commandPoolCreateInfo.pNext = NULL;
	commandPoolCreateInfo.flags = 0;
	commandPoolCreateInfo.queueFamilyIndex = pRenderDevice->graphicsFamilyIndex;

	if (vkCreateCommandPool(pRenderDevice->device, &commandPoolCreateInfo, NULL, &pRenderDevice->commandPool) != VK_SUCCESS)
	{
		return false;
	}

	VkDescriptorPoolSize descriptorPoolSizes[2] = {0};
	descriptorPoolSizes[0].type = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
	descriptorPoolSizes[0].descriptorCount = 1;
	descriptorPoolSizes[1].type = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
	descriptorPoolSizes[1].descriptorCount = 1;

	VkDescriptorPoolCreateInfo descriptorPoolCreateInfo = {0};
	descriptorPoolCreateInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
	descriptorPoolCreateInfo.pNext = NULL;
	descriptorPoolCreateInfo.flags = 0;
	descriptorPoolCreateInfo.maxSets = 3;
	descriptorPoolCreateInfo.poolSizeCount = 2;
	descriptorPoolCreateInfo.pPoolSizes = descriptorPoolSizes;

	if (vkCreateDescriptorPool(pRenderDevice->device, &descriptorPoolCreateInfo, NULL, &pRenderDevice->descriptorPool) != VK_SUCCESS)
	{
		return false;
	}

	return true;
}

void FRenderDeviceVulkanDestroyPools(FRenderDeviceVulkan* pRenderData)
{
	if (pRenderData == NULL)
	{
		return;
	}

	if (pRenderData->descriptorPool != VK_NULL_HANDLE)
	{
		vkDestroyDescriptorPool(pRenderData->device, pRenderData->descriptorPool, NULL);
	}
	
	if (pRenderData->commandPool != VK_NULL_HANDLE)
	{
		vkDestroyCommandPool(pRenderData->device, pRenderData->commandPool, NULL);
	}
}

VkCommandBuffer FRenderDeviceVulkanBeginTemporaryCommandBuffer(FRenderDeviceVulkan* pRenderDevice)
{
	if (pRenderDevice == NULL)
	{
		return VK_NULL_HANDLE;
	}

	/* Allocate the new command buffer */
	VkCommandBufferAllocateInfo allocateInfo = {0};
	allocateInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
	allocateInfo.pNext = NULL;
	allocateInfo.commandPool = pRenderDevice->commandPool;
	allocateInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
	allocateInfo.commandBufferCount = 1;

	VkCommandBuffer temporaryCommandBuffer = VK_NULL_HANDLE;
	if (vkAllocateCommandBuffers(pRenderDevice->device, &allocateInfo, &temporaryCommandBuffer) != VK_SUCCESS)
	{
		return VK_NULL_HANDLE;
	}

	/* Begin the temporary command buffer */
	VkCommandBufferBeginInfo beginInfo = {0};
	beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
	beginInfo.pNext = NULL;
	beginInfo.flags = 0;
	beginInfo.pInheritanceInfo = NULL;

	if (vkBeginCommandBuffer(temporaryCommandBuffer, &beginInfo) != VK_SUCCESS)
	{
		vkFreeCommandBuffers(pRenderDevice->device, pRenderDevice->commandPool, 1, &temporaryCommandBuffer); /* Free on error */
		return VK_NULL_HANDLE;
	}

	return temporaryCommandBuffer;
}

void FRenderDeviceVulkanEndTemporaryCommandBuffer(FRenderDeviceVulkan* pRenderDevice, VkCommandBuffer temporaryCommandBuffer)
{
	if (pRenderDevice == NULL || temporaryCommandBuffer == VK_NULL_HANDLE)
	{
		return;
	}

	vkEndCommandBuffer(temporaryCommandBuffer);

	VkSubmitInfo submitInfo = {0};
	submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
	submitInfo.pNext = NULL;
	submitInfo.waitSemaphoreCount = 0;
	submitInfo.pWaitSemaphores = NULL;
	submitInfo.pWaitDstStageMask = NULL;
	submitInfo.commandBufferCount = 0;
	submitInfo.pCommandBuffers = &temporaryCommandBuffer;
	submitInfo.signalSemaphoreCount = 0;
	submitInfo.pSignalSemaphores = NULL;

	vkQueueSubmit(pRenderDevice->graphicsQueue, 1, &submitInfo, VK_NULL_HANDLE);

	vkFreeCommandBuffers(pRenderDevice->device, pRenderDevice->commandPool, 1, &temporaryCommandBuffer);
}

FBufferVulkan* FRenderDeviceVulkanCreateBuffer(FRenderDeviceVulkan* pRenderDevice, FInt size, VkBufferUsageFlagBits usageFlags, VmaMemoryUsage memoryUsage)
{
	if (pRenderDevice == NULL || size <= 0)
	{
		return false;
	}

	/* Allocate the buffer */
	FBufferVulkan* pBuffer = FAllocateZero(1, sizeof(FBufferVulkan));
	if (pBuffer == NULL)
	{
		return NULL;
	}

	VkBufferCreateInfo bufferCreateInfo = {0};
	bufferCreateInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
	bufferCreateInfo.pNext = NULL;
	bufferCreateInfo.flags = 0;
	bufferCreateInfo.size = size;
	bufferCreateInfo.usage = usageFlags;
	bufferCreateInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;
	bufferCreateInfo.queueFamilyIndexCount = 0;
	bufferCreateInfo.pQueueFamilyIndices = NULL;

	VmaAllocationCreateInfo allocationCreateInfo = {0};
	allocationCreateInfo.flags = 0;
	allocationCreateInfo.usage = memoryUsage;
	allocationCreateInfo.requiredFlags = 0;
	allocationCreateInfo.preferredFlags = 0;
	allocationCreateInfo.memoryTypeBits = 0;
	allocationCreateInfo.pool = VK_NULL_HANDLE;
	allocationCreateInfo.pUserData = NULL;
	allocationCreateInfo.priority = 1.0f;

	if (vmaCreateBuffer(pRenderDevice->allocator, &bufferCreateInfo, &allocationCreateInfo, &pBuffer->buffer, &pBuffer->allocation, NULL) != VK_SUCCESS)
	{
		FRenderDeviceVulkanDestroyBuffer(pRenderDevice, &pBuffer);
		return NULL;
	}

	return pBuffer;
}

void FRenderDeviceVulkanDestroyBuffer(FRenderDeviceVulkan* pRenderDevice, FBufferVulkan** ppBuffer)
{
	if (pRenderDevice == NULL || ppBuffer == NULL || *ppBuffer == NULL)
	{
		return;
	}

	FBufferVulkan* pBuffer = *ppBuffer;

	vmaDestroyBuffer(pRenderDevice->allocator, pBuffer->buffer, pBuffer->allocation);
}