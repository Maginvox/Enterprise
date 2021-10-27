/* Copyright © 2021 Caden Miller, All Rights Reserved. */

#ifndef __FMESH_CONVERSION_H__
#define __FMESH_CONVERSION_H__

#include "Core/enTypes.h"
#include "Core/enMatrix.h"
#include "Resource/enAsset.h"
#include "Graphics/enVertex.h"

typedef enum
{
    enVertexType_Static,
    enVertexType_Skinned
} enVertexType;

typedef enum
{
    enPrimitiveType_Triangles,
    enPrimitiveType_TriangleStrip,
    enPrimitiveType_TriangleFan,
    enPrimitiveType_Lines,
    enPrimitiveType_LineStrip,
    enPrimitiveType_Points
} enPrimitiveType;

typedef struct
{
    char name[64];
    enMatrix4 transform;

    struct enNode* pParent;
    uint32 childerenCount;
    struct enNode* pChildren;

    uint32 meshCount;
    unsigned int* pMeshIndices;
} enNode;

typedef struct
{
    enVertexType type;
    enPrimitiveType primitiveType;
    uint32 vertexCount;
    enVertex* pVertices;

    uint32 indexCount;
    unsigned int* pIndices;
} enMesh;

typedef struct enVectorKey
{
    float time;
    enVector3 value;
} enVectorKey;


typedef struct enQuaternionKey
{
    float time;
    enQuaternion value;
} enQuaternionKey;

typedef struct
{
    enQuaternionKey key;
} enAnimationChannel;

typedef struct
{
    char name[64];
} enMeshAnimation;

typedef struct
{
    enNode* pRootNode;

    uint32 meshCount;
    enMesh* pMeshes;
} enMeshScene;

enMesh* enMeshParse(enAsset* pAsset);

#endif