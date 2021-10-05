#ifndef __FPIPELINE_H__
#define __FPIPELINE_H__

#include "Core/enVector.h"
#include "Core/enMatrix.h"

#include "Resource/enAsset.h"

typedef void* enPipeline;

typedef struct
{
    enMatrix4 view;
    enMatrix4 projection;
    float time;
    float resolution; /* 0.0f to 1.0f */
} enGlobalUniform;

typedef enum
{
    enPipelinePass_General,
    enPipelinePass_Lighting
} enPipelinePass;

enPipeline enPipelineCreate(enAsset* pVertex, enAsset* pFragment);
void enPipelineDestroy(enPipeline pipeline);

#endif