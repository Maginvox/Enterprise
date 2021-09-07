#include "Graphics/FRenderContext.h"
#include "FRenderContext_Impl.h"

#ifdef ENTERPRISE_GRAPHICS_HAS_VULKAN
    #include "Graphics/Context/Vulkan/FRenderContextVulkan.h"
#endif

FRenderContext* FRenderContextCreate(const FRenderContextCreateInfo* pInfo)
{
    if (pInfo == NULL)
    {
        return NULL;
    }

    /* Initialize the platform */
    

    switch(pInfo->api)
    {
        case FRenderAPI_Vulkan:
            
            break;
        
        case FRenderAPI_D3D12:
            break;
    }
}