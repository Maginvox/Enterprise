/* Copyright © 2021 Caden Miller, All Rights Reserved. */

#ifndef __FGRAPHICSCONTEXT_H__
#define __FGRAPHICSCONTEXT_H__


typedef enum FRenderAPI
{
    FRenderAPI_Vulkan,
    FRenderAPI_D3D12,
    FRenderAPI_Max = FRenderAPI_D3D12
} FRenderAPI;

typedef struct FGraphicsOptions
{
    FRenderAPI api;
} FGraphicsOptions;

typedef struct FGraphicsContext FGraphicsContext;

typedef void (*FGraphicsContextDestroyFunction)(FGraphicsContext** ppContext);
typedef void (*FGraphicsContextGetOptionsFunction)(FGraphicsContext* pContext, FGraphicsOptions* pOptions);
typedef void (*FGraphicsContextApplyOptionsFunction)(FGraphicsContext* pContext, const FGraphicsOptions* pOptions);

FGraphicsContext* FGraphicsContextCreate(const FRenderAPI api);
void FGraphicsContextDestroy(FGraphicsContext** ppContext);

void FGraphicsContextGetOptions(FGraphicsContext* pContext, FGraphicsOptions* pOptions);
void FGraphicsContextApplyOptions(FGraphicsContext* pContext, const FGraphicsOptions* pOptions);

#endif