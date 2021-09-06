#ifndef __FRENDERDEVICE_H__
#define __FRENDERDEVICE_H__

typedef enum FRenderContextAPI
{
    FRenderContextAPI_Vulkan,
} FRenderContextAPI;

typedef struct FRenderContextCreateInfo
{
    FRenderContextAPI api;
} FRenderContextCreateInfo;

typedef struct FRenderContext FRenderContext;

FRenderContext* FRenderContextCreate(const FRenderContextCreateInfo* pInfo);
void FRenderContextDestroy(FRenderContext** ppContext);

#endif