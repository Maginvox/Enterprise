#include <vulkan/vulkan.h>

#include "Core/FMemory.h"
#include "Core/FLog.h"

#include "Graphics/FVertex.h"
#include "Graphics/FPipeline.h"

#include "FPipelineVulkan.h"
#include "FGraphicsVulkan.h"



FPipeline FPipelineCreate(FAsset* pVertex, FAsset* pFragment)
{

    FPipelineVulkan* pPipeline = FAllocateZero(1, sizeof(FPipelineVulkan));
    if (pPipeline == NULL)
    {
        return NULL;
    }

    /* Create the pipeline layout */
    VkDescriptorSetLayout descriptorLayouts[3] =
    {
        graphics_vk.globalLayout
    };
    
    VkPipelineLayoutCreateInfo layoutCreateInfo =
    {
        .sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO,
        .pNext = NULL,
        .flags = 0,
        .setLayoutCount = 1,
        .pSetLayouts = descriptorLayouts
    };

    if (vkCreatePipelineLayout(graphics_vk.device, &layoutCreateInfo, NULL, &pPipeline->layout) != VK_SUCCESS)
    {
        FPipelineDestroy(pPipeline);
        FLogError("Could not create a vulkan pipeline layout!");
        return NULL;
    }

    /* Create the graphics pipeline */
    VkShaderModule vertexShader = VK_NULL_HANDLE;
    VkShaderModule fragmentShader = VK_NULL_HANDLE;

    VkShaderModuleCreateInfo shaderModuleCreateInfos[2] =
    {
        {
            .sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO,
            .pNext = NULL,
            .flags = 0,
            .codeSize = pVertex->size,
            .pCode = (const uint32_t*)pVertex->pData
        },
        {
            .sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO,
            .pNext = NULL,
            .flags = 0,
            .codeSize = pFragment->size,
            .pCode = (const uint32_t*)pFragment->pData
        }
    };

    if (vkCreateShaderModule(graphics_vk.device, &shaderModuleCreateInfos[0], NULL, &vertexShader) != VK_SUCCESS)
    {
        FPipelineDestroy(pPipeline);
        FLogError("Could not compile a vulkan vertex shader!");
        return NULL;
    }

    if (vkCreateShaderModule(graphics_vk.device, &shaderModuleCreateInfos[1], NULL, &fragmentShader) != VK_SUCCESS)
    {
        vkDestroyShaderModule(graphics_vk.device, vertexShader, NULL);
        FPipelineDestroy(pPipeline);
        FLogError("Could not compile a vulkan vertex shader!");
        return NULL;
    }

    VkPipelineShaderStageCreateInfo stages[2] =
    {
        {
            .sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO,
            .pNext = NULL,
            .flags = 0,
            .stage = VK_SHADER_STAGE_VERTEX_BIT,
            .module = vertexShader,
            .pName = "main",
            .pSpecializationInfo = NULL
        },
        {
            .sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO,
            .pNext = NULL,
            .flags = 0,
            .stage = VK_SHADER_STAGE_FRAGMENT_BIT,
            .module = fragmentShader,
            .pName = "main",
            .pSpecializationInfo = NULL
        }
    };

    VkVertexInputBindingDescription bindings[2] =
    {
        {
            .binding = 0,
            .stride = sizeof(FVertex),
            .inputRate = VK_VERTEX_INPUT_RATE_VERTEX
        },
        {
            .binding = 0,
            .stride = sizeof(FVertexSkinned),
            .inputRate = VK_VERTEX_INPUT_RATE_VERTEX
        }
    };

    VkVertexInputAttributeDescription attributeDescriptions[2][5] =
    {
        {
            { /* Location */
                .location = 0,
                .binding = 0,
                .format = VK_FORMAT_R32G32B32_SFLOAT,
                .offset = 0
            },
            { /* Normal */
                .location = 1,
                .binding = 0,
                .format = VK_FORMAT_R32G32B32_SFLOAT,
                .offset = offsetof(FVertex, normal)
            },
            { /* TexCoords */
                .location = 2,
                .binding = 0,
                .format = VK_FORMAT_R32G32_SFLOAT,
                .offset = offsetof(FVertex, texCoord)
            }
        },
        {
            { /* Location */
                .location = 0,
                .binding = 0,
                .format = VK_FORMAT_R32G32B32_SFLOAT,
                .offset = 0
            },
            { /* Normal */
                .location = 1,
                .binding = 0,
                .format = VK_FORMAT_R32G32B32_SFLOAT,
                .offset = offsetof(FVertex, normal)
            },
            { /* TexCoords */
                .location = 2,
                .binding = 0,
                .format = VK_FORMAT_R32G32_SFLOAT,
                .offset = offsetof(FVertex, texCoord)
            },
            { /* SkinWeights */
                .location = 3,
                .binding = 0,
                .format = VK_FORMAT_R32G32B32A32_SFLOAT,
                .offset = offsetof(FVertexSkinned, skinWeights)
            },
            { /* SkinIndices */
                .location = 4,
                .binding = 0,
                .format = VK_FORMAT_R32G32B32A32_SFLOAT,
                .offset = offsetof(FVertexSkinned, skinIndices)
            }
        }
    };

    VkPipelineVertexInputStateCreateInfo vertex[2] =
    {
        {
            .sType = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO,
            .pNext = NULL,
            .flags = 0,
            .vertexBindingDescriptionCount = 1,
            .pVertexBindingDescriptions = &bindings[0],
            .vertexAttributeDescriptionCount = 3,
            .pVertexAttributeDescriptions = attributeDescriptions[0]
        },
        {
            .sType = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO,
            .pNext = NULL,
            .flags = 0,
            .vertexBindingDescriptionCount = 1,
            .pVertexBindingDescriptions = &bindings[1],
            .vertexAttributeDescriptionCount = 5,
            .pVertexAttributeDescriptions = attributeDescriptions[1]  
        }
    };

    VkPipelineInputAssemblyStateCreateInfo assemblyState = 
    {
        .sType = VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO,
        .pNext = NULL,
        .flags = 0,
        .topology = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST,
        .primitiveRestartEnable = VK_FALSE
    };

    VkPipelineRasterizationStateCreateInfo rasterizer =
    {
        .sType = VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO,
        .pNext = NULL,
        .flags = 0,
        .depthClampEnable = VK_FALSE,
        .rasterizerDiscardEnable = VK_FALSE,
        .polygonMode = VK_POLYGON_MODE_FILL,
        .cullMode = VK_CULL_MODE_BACK_BIT,
        .frontFace = VK_FRONT_FACE_CLOCKWISE,
        .depthBiasEnable = VK_FALSE,
        .depthBiasConstantFactor = 0.0f,
        .depthBiasClamp = 0.0f,
        .depthBiasSlopeFactor = 0.0f,
        .lineWidth = 0.0f 
    };

    VkPipelineColorBlendAttachmentState colorAttachment = {
        .blendEnable = VK_TRUE,
        .srcColorBlendFactor = VK_BLEND_FACTOR_SRC_ALPHA,
        .dstColorBlendFactor = VK_BLEND_FACTOR_ONE_MINUS_SRC_ALPHA,
        .colorBlendOp = VK_BLEND_OP_ADD,
        .srcAlphaBlendFactor = VK_BLEND_FACTOR_ONE,
        .dstAlphaBlendFactor = VK_BLEND_FACTOR_ZERO,
        .alphaBlendOp = VK_BLEND_OP_ADD
    };

    VkPipelineColorBlendStateCreateInfo colorBlend =
    {
        .sType = VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO,
        .pNext = NULL,
        .flags = 0,
        .logicOpEnable = VK_FALSE,
        .logicOp = VK_LOGIC_OP_COPY,
        .attachmentCount = 1,
        .pAttachments = &colorAttachment,
        .blendConstants = {0.0f, 0.0f, 0.0f, 0.0f}
    };

    VkDynamicState dynamicStates[2] =
    {
        VK_DYNAMIC_STATE_SCISSOR,
        VK_DYNAMIC_STATE_VIEWPORT
    };

    VkPipelineDynamicStateCreateInfo dynamicState =
    {
        .sType = VK_STRUCTURE_TYPE_PIPELINE_DYNAMIC_STATE_CREATE_INFO,
        .pNext = NULL,
        .flags = 0,
        .dynamicStateCount = 2,
        .pDynamicStates = dynamicStates
    };

    VkGraphicsPipelineCreateInfo pipelineCreateInfo = {
        .sType = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO,
        .pNext = NULL,
        .flags = 0,
        .stageCount = 2,
        .pStages = stages,
        .pVertexInputState = &vertex[0],
        .pInputAssemblyState = &assemblyState,
        .pTessellationState = NULL,
        .pViewportState = NULL,
        .pRasterizationState = &rasterizer,
        .pMultisampleState = NULL,
        .pDepthStencilState = NULL,
        .pColorBlendState = &colorBlend,
        .pDynamicState = &dynamicState,
        .layout = pPipeline->layout,
        .renderPass = graphics_vk.generalPass,
        .subpass = 0,
        .basePipelineHandle = VK_NULL_HANDLE,
        .basePipelineIndex = 0
    };

    if (vkCreateGraphicsPipelines(graphics_vk.device, VK_NULL_HANDLE, 1, &pipelineCreateInfo, NULL, &pPipeline->pipeline) != VK_SUCCESS)
    {
        FPipelineDestroy(pPipeline);
        FLogError("Could not create a vulkan graphics pipeline!");
        return NULL;
    }
    
    vkDestroyShaderModule(graphics_vk.device, vertexShader, NULL);
    vkDestroyShaderModule(graphics_vk.device, fragmentShader, NULL);

    return pPipeline;
}

void FPipelineDestroy(FPipeline pipeline)
{
    FPipelineVulkan* pPipeline = pipeline;

    if (pPipeline->pipeline != VK_NULL_HANDLE)
    {
        vkDestroyPipeline(graphics_vk.device, pPipeline->pipeline, NULL);
    }

    if (pPipeline->layout != VK_NULL_HANDLE)
    {
        vkDestroyPipelineLayout(graphics_vk.device, pPipeline->layout, NULL);
    }

    FDeallocate(pPipeline);
}