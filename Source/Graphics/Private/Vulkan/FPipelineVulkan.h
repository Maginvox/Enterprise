#ifndef __FPIPELINE_VULKAN_H__
#define __FPIPELINE_VULKAN_H__

#include <vulkan/vulkan.h>

typedef struct
{
    VkPipelineLayout layout;
    VkPipeline pipeline;
} FPipelineVulkan;

#endif