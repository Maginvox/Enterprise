#ifndef __FRENDERCONTEXT_IMPL_H__
#define __FRENDERCONTEXT_IMPL_H__

#include "Graphics/FRenderContext.h"
#include "Graphics/FRenderPass.h"
#include "Graphics/FImage.h"
#include "Graphics/FFramebuffer.h"

typedef struct FRenderContext
{
    FRenderContext* pRenderContext;

    FRenderContextDestroyFunction pRenderContextDestroy;

    FRenderPassCreateFunction pRenderPassCreate;
    FRenderPassDestroyFunction pRenderPassDestroy;
    
    FImageCreateFunction pImageCreate;
    FImageDestroyFunction pImageDestroy;

    FFramebufferCreateFunction pFramebufferCreate;
    FFramebufferDestroyFunction pFramebufferDestroy;
} FRenderContext;

#endif