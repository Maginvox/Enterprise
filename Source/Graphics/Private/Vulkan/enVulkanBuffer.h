#ifndef __ENTERPRISE_VULKAN_BUFFER_H__
#define __ENTERPRISE_VULKAN_BUFFER_H__

#include <vulkan/vulkan.h>
#include <vk_mem_alloc.h>

typedef struct
{
    VkBuffer buffer;
    VmaAllocation allocation;
} *enVulkanBuffer;

enVulkanBuffer enVulkanBufferCreate(VkDeviceSize size, VkBufferUsageFlags usage, VmaMemoryUsage memoryUsage);
void enVulkanBufferDestroy(enVulkanBuffer buffer);
void enVulkanBufferUpdate(enVulkanBuffer buffer, void* data, VkDeviceSize size);

#endif