#ifndef __FRENDERCONTEXT_IMPL_H__
#define __FRENDERCONTEXT_IMPL_H__

#include "Graphics/FRenderContext.h"
#include "Graphics/FRenderPass.h"
#include "Graphics/FImage.h"
#include "Graphics/FFramebuffer.h"

typedef struct FRenderContext
{
    FRenderData* pRenderData;

    PFN_FRenderPassCreate pRenderPassCreate;
    PFN_FRenderPassDestroy pRenderPassDestroy;
    
    PFN_FImageCreate pImageCreate;
    PFN_FImageDestroy pImageDestroy;

    PFN_FFramebufferCreate pFramebufferCreate;
    PFN_FFramebufferDestroy pFramebufferDestroy;
} FRenderContext;

#endif