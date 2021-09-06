#ifndef __FVERTEX_H__
#define __FVERTEX_H__

#include "Core/FVector.h"

typedef struct FVertex
{
    FVector3 position;
    FVector3 normal;
    FVector2 texCoord;
} FVertex;

typedef struct FVertexSkinned
{
    FVector3 position;
    FVector3 normal;
    FVector2 texCoord;
    FVector4 skinWeights;
    FVector4 skinIndices;
} FVertexSkinned;

#endif