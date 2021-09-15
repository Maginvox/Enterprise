#ifndef __VULKAN_RENDER_DATA_H__
#define __VULKAN_RENDER_DATA_H__

#include <vulkan/vulkan.h>
#include <vk_mem_alloc.h>

#include "Graphics/FRenderAPI.h"

/* Vulkan structures */
typedef struct FBufferVulkan
{
    VmaAllocation allocation;
    VkBuffer buffer;
} FBufferVulkan;

/* RenderDevice functions and structure  */
typedef struct FRenderAPIVulkan
{
    VkInstance instance;
    PFN_vkCreateDebugUtilsMessengerEXT CreateDebugUtilsMessenger;
    PFN_vkDestroyDebugUtilsMessengerEXT DestroyDebugUtilsMessenger;
    VkDebugUtilsMessengerEXT debugMessenger;
    VkSurfaceKHR surface;
    
    VkPhysicalDevice physicalDevice;
    VkDevice device;
    FInt graphicsFamilyIndex, presentFamilyIndex, computeFamilyIndex;
    VkQueue graphicsQueue, presentQueue, computeQueue;
    bool bHasCompute;
    VmaAllocator allocator;

    VkRenderPass renderPass;

    uint32_t imageCount;
    VkSurfaceFormatKHR surfaceFormat;
    VkPresentModeKHR presentMode;
    VkSwapchainKHR swapchain;
    VkImage* pSwapchainImages;
    VkImageView* pSwapchainImageViews;
    VkFramebuffer* pSwapchainFramebuffers;

    VkCommandPool commandPool;
    VkDescriptorPool descriptorPool;
} FRenderDeviceVulkan;

typedef struct FTextureVulkan
{
    VkImage image;
    VkImageView view;
    VmaAllocation allocation;
} FTextureVulkan;

typedef struct FMeshVulkan
{
    FBufferVulkan* pBuffer;
} FMeshVulkan;

typedef struct FMaterialVulkan
{
    VkSampler sampler;
    VkImageView view;
    VkDescriptorImageInfo descriptor;
} FMaterialVulkan;

typedef struct FMaterialInstanceVulkan
{
    VkSampler sampler;
    VkImageView view;
    VkDescriptorImageInfo descriptor;

    VkDescriptorSet descriptorSet;
    VkDescriptorSetLayout descriptorSetLayout;
    VkPipelineLayout pipelineLayout;
    VkPipeline pipeline;
} FMaterialInstanceVulkan;


void FRenderVulkanLoad(FRenderer* pRenderer);
bool FRenderDeviceVulkanCreate(FRenderer* pRenderer);
void FRenderDeviceVulkanDestroy(FRenderDevice** ppDevice);

FTexture* FTextureVulkanCreate(FRenderDevice* pRenderDevice, const FTextureCreateOptions* pOptions);
void FTextureVulkanDestroy(FRenderDevice* pRenderDevice, FTexture** ppTexture);
void FTextureVulkanUpdate(FRenderDevice* pRenderDevice, FTexture* pTexture, const char* pData);

FMesh* FMeshVulkanCreate(FRenderDevice* pRenderDevice, const FMeshCreateOptions* pOptions);
void FMeshVulkanDestroy(FRenderDevice* pRenderDevice, FMesh** ppMesh);

FMaterial* FMaterialVulkanCreate(FRenderDevice* pRenderDevice, const FMaterialCreateOptions* pOptions);
void FMaterialVulkanDestroy(FRenderDevice* pRenderDevice, FMaterial** ppMaterial);

FMaterialInstance* FMaterialInstanceVulkanCreate(FRenderDevice* pRenderDevice, FMaterial* pMaterial);
void FMaterialInstanceVulkanDestroy(FRenderDevice* pRenderDevice, FMaterialInstance** ppMaterialInstance);
void FMaterialInstanceVulkanSetFloat(FRenderDevice* pRenderDevice, FMaterialInstance* pMaterialInstance, const char* pName, float value);
void FMaterialInstanceVulkanSetVector2(FRenderDevice* pRenderDevice, FMaterialInstance* pMaterialInstance, const char* pName, const FVector2* value);
void FMaterialInstanceVulkanSetVector3(FRenderDevice* pRenderDevice, FMaterialInstance* pMaterialInstance, const char* pName, const FVector3* value);
void FMaterialInstanceVulkanSetVector4(FRenderDevice* pRenderDevice, FMaterialInstance* pMaterialInstance, const char* pName, const FVector4* value);
void FMaterialInstanceVulkanSetMatrix4(FRenderDevice* pRenderDevice, FMaterialInstance* pMaterialInstance, const char* pName, const FMatrix4* value);
void FMaterialInstanceVulkanSetTexture(FRenderDevice* pRenderDevice, FMaterialInstance* pMaterialInstance, const char* pName, FTexture* pTexture);

/* Callbacks */
void FRenderDeviceVulkanSurfaceResizeCallback(FRenderDevice* pRenderDevice, FInt width, FInt height);
 

/* Vulkan render device functions */
const char* const* FVulkanGetExtensions(uint32_t* pCount);
const char* const* FVulkanGetLayers(uint32_t* pCount);
const char* const* FVulkanGetDeviceExtensions(FRenderDeviceVulkan* pRenderData, uint32_t* pCount);

VkBool32 FVulkanDebugCallback(VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity, VkDebugUtilsMessageTypeFlagsEXT messageTypes, const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData, void* pUserData);

bool FRenderDeviceVulkanCreateInstance(FRenderDeviceVulkan* pRenderDeviceVulkan, bool bDebug);
void FRenderDeviceVulkanDestroyInstance(FRenderDeviceVulkan* pRenderDeviceVulkan);
bool FRenderDeviceVulkanCreateSurface(FRenderer* pRenderer, FRenderDeviceVulkan* pRenderDeviceVulkan);
void FRenderDeviceVulkanDestroySurface(FRenderDeviceVulkan* pRenderDeviceVulkan);
bool FRenderDeviceVulkanChoosePhysicalDevice(FRenderDeviceVulkan* pRenderDeviceVulkan, FInt id);
bool FRenderDeviceVulkanChooseFormats(FRenderDeviceVulkan* pRenderDeviceVulkan);
bool FRenderDeviceVulkanChooseQueueFamilies(FRenderDeviceVulkan* pRenderDeviceVulkan);
bool FRenderDeviceVulkanCreateDevice(FRenderDeviceVulkan* pRenderDeviceVulkan, bool bDebug);
void FRenderDeviceVulkanDestroyDevice(FRenderDeviceVulkan* pRenderDeviceVulkan);
bool FRenderDeviceVulkanCreateAllocator(FRenderDeviceVulkan* pRenderDeviceVulkan);
void FRenderDeviceVulkanDestroyAllocator(FRenderDeviceVulkan* pRenderDeviceVulkan);
bool FRenderDeviceVulkanCreatePools(FRenderDeviceVulkan* pRenderDeviceVulkan);
void FRenderDeviceVulkanDestroyPools(FRenderDeviceVulkan* pRenderDeviceVulkan);
bool FRenderDeviceVulkanCreateRenderpasses(FRenderDeviceVulkan* pRenderDeviceVulkan);
void FRenderDeviceVulkanDestroyRenderpasses(FRenderDeviceVulkan* pRenderDeviceVulkan);
bool FRenderDeviceVulkanCreateSwapchain(FRenderDeviceVulkan* pRenderDeviceVulkan, VkExtent2D extent);
void FRenderDeviceVulkanDestroySwapchain(FRenderDeviceVulkan* pRenderDeviceVulkan);

VkCommandBuffer FRenderDeviceVulkanBeginTemporaryCommandBuffer(FRenderDeviceVulkan* pRenderDevice);
void FRenderDeviceVulkanEndTemporaryCommandBuffer(FRenderDeviceVulkan* pRenderDevice, VkCommandBuffer commandBuffer); /* Also submits the command buffer */
FBufferVulkan* FRenderDeviceVulkanCreateBuffer(FRenderDeviceVulkan* pRenderDevice, FInt size, VkBufferUsageFlagBits usageFlags, VmaMemoryUsage memoryUsage);
void FRenderDeviceVulkanDestroyBuffer(FRenderDeviceVulkan* pRenderDevice, FBufferVulkan** ppBuffer);

#endif