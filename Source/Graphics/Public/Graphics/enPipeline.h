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

typedef enum enPipelinePass
{
    enPipelinePass_General,
    enPipelinePass_Lighting
} enPipelinePass;

typedef enum enUniformType
{
    enUniformType_Buffer,
    enUniformType_Texture,
    enUniformType_Sampler
} enUniformType;

typedef struct enUniformDesc
{
    enUniformType type;
    const char* pName;
    uint64 size;
} enUniformDesc;

enPipeline enPipelineCreate(enPipelinePass pass, enAsset* pVertex, enAsset* pFragment);
void enPipelineDestroy(enPipeline pipeline);

#endif