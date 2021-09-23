/* Copyright © 2021 Caden Miller, All Rights Reserved. */

#include "Core/FMath.h"
#include "Core/FLog.h"
#include "Graphics/FGraphicsContext.h"
#include "FGraphicsContext_Impl.h"

#ifdef ENTERPRISE_GRAPHICS_VULKAN
    #include "Context/Vulkan/FGraphicsContextVulkan.h"
#endif

FGraphicsContext* FGraphicsContextCreate(FWindow* pWindow, const FGraphicsOptions* pOptions)
{
    if (!FMathIsBetween(pOptions->api, 0, FRenderAPI_Max))
    {
        return NULL;
    }

    switch(pOptions->api)
    {

#ifdef ENTERPRISE_GRAPHICS_VULKAN
        case FRenderAPI_Vulkan:
            return FGraphicsContextVulkanCreate(pWindow, pOptions);
            break;
#endif

#ifdef ENTERPRISE_GRAPHICS_D3D12
        case FRenderAPI_D3D12:
            break;
#endif

        default:
            return NULL;
    }

    return NULL;
}

void FGraphicsContextDestroy(FGraphicsContext** ppContext)
{
    if (ppContext == NULL || *ppContext == NULL)
    {
        return;
    }
    
    

    FGraphicsContext* pContext = *ppContext;

}