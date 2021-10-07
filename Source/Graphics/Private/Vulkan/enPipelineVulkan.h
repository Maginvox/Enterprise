#ifndef __ENTERPRISE_PIPELINE_VULKAN_H__
#define __ENTERPRISE_PIPELINE_VULKAN_H__

#include <vulkan/vulkan.h>

typedef struct
{
    VkPipelineLayout layout;
    VkPipeline pipeline;
} enPipelineVulkan;

#endif