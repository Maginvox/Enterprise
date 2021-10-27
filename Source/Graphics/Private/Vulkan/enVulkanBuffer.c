#include "Core/enMemory.h"
#include "enGraphicsVulkan.h"

#include "enVulkanBuffer.h"

enVulkanBuffer* enVulkanBufferCreate(VkDeviceSize size, VkMemoryPropertyFlagBits properties, VkBufferUsageFlags usage, VmaMemoryUsage memoryUsage)
{
    enVulkanBuffer* buffer = enCalloc(1, sizeof(enVulkanBuffer));
    if (buffer == NULL)
    {
        return NULL;
    }

    VkBufferCreateInfo createInfo = {
        .sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO,
        .pNext = NULL,
        .flags = 0,
        .size = size,
        .usage = usage,
        .sharingMode = VK_SHARING_MODE_EXCLUSIVE,
        .queueFamilyIndexCount = 0,
        .pQueueFamilyIndices = NULL
    };

    VmaAllocationCreateInfo allocInfo = {
        .flags = 0,
        .usage = memoryUsage,
        .requiredFlags = properties,
        .preferredFlags = 0,
        .memoryTypeBits = 0,
        .pool = VK_NULL_HANDLE,
        .pUserData = NULL
    };

    if (vmaCreateBuffer(graphics_vk.allocator, &createInfo, &allocInfo, &buffer->buffer, &buffer->allocation, NULL) != VK_SUCCESS)
    {
        enVulkanBufferDestroy(buffer);
        return NULL;
    }

    return buffer;
}

void enVulkanBufferDestroy(enVulkanBuffer* buffer)
{
    if (buffer == NULL)
    {
        return;
    }

    vmaDestroyBuffer(graphics_vk.allocator, buffer->buffer, buffer->allocation);
    enFree(buffer);
}

void enVulkanBufferUpdate(enVulkanBuffer* buffer, void* data, VkDeviceSize size)
{
    if (buffer == NULL || data == NULL)
    {
        return;
    }

    void* pMapped = NULL;
    if (vmaMapMemory(graphics_vk.allocator, buffer->allocation, &pMapped) != VK_SUCCESS)
    {
        return;
    }
        enMemoryCopy(pMapped, data, size);
    vmaUnmapMemory(graphics_vk.allocator, buffer->allocation);
}