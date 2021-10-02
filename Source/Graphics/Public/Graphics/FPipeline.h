#ifndef __FPIPELINE_H__
#define __FPIPELINE_H__

#include "Resource/FAsset.h"

typedef void* FPipeline;

typedef enum
{
    FPipelinePass_General,
    FPipelinePass_Lighting
} FPipelinePass;

FPipeline FPipelineCreate(FAsset* pVertex, FAsset* pFragment);
void FPipelineDestroy(FPipeline pipeline);

#endif