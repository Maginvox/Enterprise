#ifdef ENTERPRISE_WINDOWS

#include "Rendering/D3D12/d3d12renderdata.h"

void FD3D12RenderDataLoad(FRenderer* pContext)
{
    if (pContext == NULL)
    {
        return;
    }

    pContext->pRenderDeviceDestroy = NULL;
}

bool FD3D12RenderDataCreate(FRenderer* pContext)
{
    if (pContext == NULL)
    {
        return false;
    }

    FD3D12RenderData* pD3D12RenderData = calloc(1, sizeof(FD3D12RenderData));
    if (pD3D12RenderData == NULL)
    {
        return false;
    }


    return true;
}

void FD3D12RenderDataDestroy(FRenderDevice* pRenderData)
{

}

bool FD3D12RenderDataDeviceCreate(FD3D12RenderData* pRenderData)
{
    if (pRenderData == NULL)
    {
        return false;
    }

    return true;
}

void FD3D12RenderDataDeviceDestroy(FD3D12RenderData* pRenderData)
{
    
}

#endif