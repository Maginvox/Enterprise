#ifndef __FCOMMANDS_H__
#define __FCOMMANDS_H__

#include "Core/FTypes.h"

typedef void* FCommandBuffer;

typedef struct
{
    FMatrix4 model;
} FInstanceData;

FCommandBindMaterial(FCommandBuffer buffer, FMaterialInstance material);
FCommandBindMesh(FCommandBuffer buffer, FMesh mesh);

FCommandDrawIndexed(FCommandBuffer buffer, FUInt count, FInstanceData* pInstanceDatas);

FCommandPresent(FCommandBuffer buffer, FContext context);

#endif