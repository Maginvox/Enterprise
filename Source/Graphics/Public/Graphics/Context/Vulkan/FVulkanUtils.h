#ifndef __FVULKAN_UTILS_H__
#define __FVULKAN_UTILS_H__

#include <vulkan/vulkan.h>

#include "Graphics/FImage.h"
#include "Graphics/FRenderPass.h"

static inline VkFormat FImageFormatToVkFormat(FImageFormat format)
{
    switch (format)
    {
        case FImageFormat_SRGB8:
            return VK_FORMAT_R8G8B8A8_SRGB;
            break;
        default:
            return VK_FORMAT_UNDEFINED;
    }
}

static inline VkAttachmentLoadOp FRenderPassAttachmentLoadOpToVkAttachmentLoadOp(FRenderPassAttachmentLoadOp loadOp)
{
    switch (loadOp)
    {
        case FRenderPassAttachmentLoadOp_Load:
            return VK_ATTACHMENT_LOAD_OP_LOAD;
            break;
        case FRenderPassAttachmentLoadOp_DontCare:
            return VK_ATTACHMENT_LOAD_OP_DONT_CARE;
            break;
        case FRenderPassAttachmentLoadOp_Clear:
            return VK_ATTACHMENT_LOAD_OP_CLEAR;
            break;
        default:
            return VK_ATTACHMENT_LOAD_OP_LOAD;
            break;
    }
}

static inline VkAttachmentStoreOp FRenderPassAttachmentStoreOpToVkAttachmentStoreOp(FRenderPassAttachmentStoreOp storeOp)
{
    switch (storeOp)
    {
        case FRenderPassAttachmentStoreOp_Store:
            return VK_ATTACHMENT_STORE_OP_STORE;
            break;
        case FRenderPassAttachmentStoreOp_DontCare:
            return VK_ATTACHMENT_STORE_OP_DONT_CARE;
            break;
        default:
            return VK_ATTACHMENT_STORE_OP_STORE;
            break;
    }
}

static inline VkImageLayout FImageLayoutToVkImageLayout(FImageLayout layout)
{
    switch (layout)
    {
        case FImageLayout_General:
            return VK_IMAGE_LAYOUT_GENERAL;
            break;
        case FImageLayout_Color:
            return VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;
            break;
        case FImageLayout_DepthStencil:
            return VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL;
            break;
        case FImageLayout_ReadShader:
            return VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
            break;
        case FImageLayout_Present:
            return VK_IMAGE_LAYOUT_PRESENT_SRC_KHR;
            break;
        default:
            return VK_IMAGE_LAYOUT_UNDEFINED;
            break;
    }
}

#endif