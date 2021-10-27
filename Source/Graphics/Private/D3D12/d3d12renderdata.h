/* Copyright © 2021 Caden Miller, All Rights Reserved. */

#ifndef __D3D12_RENDER_DATA_H__
#define __D3D12_RENDER_DATA_H__

#include <d3d12.h>

#include "FTypes.h"
#include "Rendering/FRenderer.h"

typedef struct FD3D12RenderData
{
    ID3D12Device *pDevice;
} FD3D12RenderData;


void FD3D12RenderDataLoad(FRenderer* pDevice);
bool FD3D12RenderDataCreate(FRenderer* pDevice);
void FD3D12RenderDataDestroy(FRenderDevice* pRenderData);

bool FD3D12RenderDataDeviceCreate(FD3D12RenderData* pRenderData);
void FD3D12RenderDataDeviceDestroy(FD3D12RenderData* pRenderData);



#endif