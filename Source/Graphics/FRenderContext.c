#include "Graphics/FRenderContext.h"
#include "FRenderContext_Impl.h"

#ifdef ENTERPRISE_GRAPHICS_VULKAN
    #include "Graphics/Context/Vulkan/FRenderContextVulkan.h"
#endif

FRenderContext* FRenderContext_Create(const FRenderContextCreateInfo* pInfo)
{
    if (pInfo == NULL)
    {
        return NULL;
    }

    /* Initialize the platform */
    

    switch(pInfo->api)
    {
        case FRenderAPI_Vulkan:
            return FRenderContextVulkan_Create(pInfo);
            break;
        
        case FRenderAPI_D3D12:
            break;

        default:
            return NULL;
    }
}