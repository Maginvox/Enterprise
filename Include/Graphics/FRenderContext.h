#ifndef __FRENDERCONTEXT_H__
#define __FRENDERCONTEXT_H__

typedef enum FRenderAPI
{
    FRenderAPI_Vulkan,
    FRenderAPI_D3D12,
} FRenderAPI;

typedef struct FRenderContextCreateInfo
{
    FRenderAPI api;
} FRenderContextCreateInfo;

typedef struct FRenderContext FRenderContext;

FRenderContext* FRenderContextCreate(const FRenderContextCreateInfo* pInfo);
void FRenderContextDestroy(FRenderContext** ppContext);

#endif