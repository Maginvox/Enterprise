#include <vulkan/vulkan.h>

#include "FGraphicsVulkan.h"

typedef void* FPipeline;

FPipeline FPipelineCreate()
{

    VkPipelineShaderStageCreateInfo stages[2] =
    {
        {
            .sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO,
            .pNext = NULL,
            .flags = 0,
            .stage = VK_SHADER_STAGE_VERTEX_BIT,
            .pName = "main",
            .pSpecializationInfo = NULL
        },
        {
            .sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO,
            .pNext = NULL,
            .flags = 0,
            .stage = VK_SHADER_STAGE_FRAGMENT_BIT,
            .pName = "main",
            .pSpecializationInfo = NULL
        }
    };

    VkGraphicsPipelineCreateInfo pipelineCreateInfo = {
        .sType = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO,
        .pNext = NULL,
        .flags = 0,
        .stageCount = 2,
        .pStages = stages,
        .
    }

    if (vkCreateGraphicsPipelines(graphics_vk.device, VK_NULL_HANDLE, 1, ) != VK_SUCCESS)
    {

    }

}

void FPipelineDestroy()
{

}