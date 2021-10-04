/* Copyright © 2021 Caden Miller, All Rights Reserved. */

#ifndef __FVERTEX_H__
#define __FVERTEX_H__

#include "Core/enVector.h"

typedef struct
{
    enVector3 position;
    enVector3 normal;
    enVector2 texCoord;
} enVertex;

typedef struct
{
    enVector3 position;
    enVector3 normal;
    enVector2 texCoord;
    enVector4 skinWeights;
    enVector4 skinIndices;
} FVertexSkinned;

#endif