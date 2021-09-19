/* Copyright © 2021 Caden Miller, All Rights Reserved. */

#ifndef __FGRAPHICSCONTEXT_IMPL_H__
#define __FGRAPHICSCONTEXT_IMPL_H__

#include "Graphics/FGraphicsContext.h"
#include "Graphics/FRenderPass.h"
#include "Graphics/FImage.h"
#include "Graphics/FFramebuffer.h"

typedef struct FGraphicsContext
{
    FGraphicsContext* pRenderContext;

    FGraphicsContextDestroyFunction pRenderContextDestroy;

    FRenderPassCreateFunction pRenderPassCreate;
    FRenderPassDestroyFunction pRenderPassDestroy;
    
    FImageCreateFunction pImageCreate;
    FImageDestroyFunction pImageDestroy;

    FFramebufferCreateFunction pFramebufferCreate;
    FFramebufferDestroyFunction pFramebufferDestroy;
} FGraphicsContext;

#endif