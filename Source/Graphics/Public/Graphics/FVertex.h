/* Copyright © 2021 Caden Miller, All Rights Reserved. */

#ifndef __FVERTEX_H__
#define __FVERTEX_H__

#include "Core/FVector.h"

typedef struct
{
    FVector3 position;
    FVector3 normal;
    FVector2 texCoord;
} FVertex;

typedef struct
{
    FVector3 position;
    FVector3 normal;
    FVector2 texCoord;
    FVector4 skinWeights;
    FVector4 skinIndices;
} FVertexSkinned;

#endif