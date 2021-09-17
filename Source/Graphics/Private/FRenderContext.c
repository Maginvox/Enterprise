#include <SDL.h>

#include "Core/FMath.h"
#include "Core/FLog.h"
#include "Graphics/FGraphicsContext.h"
#include "FGraphicsContext_Impl.h"

#ifdef ENTERPRISE_GRAPHICS_VULKAN
    #include "Context/Vulkan/FGraphicsContextVulkan.h"
#endif

FGraphicsContext* FRenderContextCreate(const FRenderAPI api)
{
    if (!FMathIsBetween(api, 0, FRenderAPI_Max))
    {
        return NULL;
    }

    if (SDL_Init(SDL_INIT_EVERYTHING) != SDL_TRUE)
    {
        return NULL;
    }
    
    switch(api)
    {

#ifdef ENTERPRISE_GRAPHICS_VULKAN
        case FRenderAPI_Vulkan:
            return FGraphicsContextVulkanCreate();
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

void FRenderContext_Destroy(FGraphicsContext** ppContext)
{
    if (ppContext == NULL || *ppContext == NULL)
    {
        return;
    }
    
    FGraphicsContext* pContext = *ppContext;
    
}