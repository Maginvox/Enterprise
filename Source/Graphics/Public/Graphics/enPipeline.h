#ifndef __FPIPELINE_H__
#define __FPIPELINE_H__

#include "Resource/enAsset.h"

typedef void* enPipeline;

typedef enum
{
    enPipelinePass_General,
    enPipelinePass_Lighting
} enPipelinePass;

enPipeline enPipelineCreate(enAsset* pVertex, enAsset* pFragment);
void enPipelineDestroy(enPipeline pipeline);

#endif