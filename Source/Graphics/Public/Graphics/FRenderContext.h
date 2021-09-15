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

typedef void (*FRenderContextCreateFunction)(const FRenderContextCreateInfo* pInfo);
typedef void (*FRenderContextDestroyFunction)(FRenderContext** ppContext);

FRenderContext* FRenderContextCreate(const FRenderContextCreateInfo* pInfo);
void FRenderContextDestroy(FRenderContext** ppContext);

#endif