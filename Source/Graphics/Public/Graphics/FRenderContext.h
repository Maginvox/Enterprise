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
    const char* pTitle;
} FRenderContextCreateInfo;

typedef struct FRenderContextOptions
{
    FRenderAPI api;
} FRenderContextOptions;

typedef struct FRenderContext FRenderContext;

FRenderContext* FRenderContext_Create(const FRenderContextCreateInfo* pInfo);
void FRenderContext_Destroy(FRenderContext** ppContext);

#endif