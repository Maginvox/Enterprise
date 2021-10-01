#ifndef __FPIPELINE_VULKAN_H__
#define __FPIPELINE_VULKAN_H__

#include <Vulkan/vulkan.h>

typedef struct
{
    VkPipelineLayout layout;
    VkPipeline pipeline;
} FPipelineVulkan;

#endif