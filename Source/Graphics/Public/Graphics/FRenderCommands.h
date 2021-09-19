/* Copyright © 2021 Caden Miller, All Rights Reserved. */

#ifndef __FRENDER_COMMANDS_H__
#define __FRENDER_COMMANDS_H__

typedef struct FInstanceData
{
    FMatrix4 transform;
} FInstanceData;


void FRenderCommandDrawMesh(const FMesh* pMesh, const FInstanceData* pInstanceData);
void FRenderCommandDrawMeshInstanced(const FMesh* pMesh, FInt32 instanceCount, FInstanceData* pInstanceData); /* pInstanceData array size must == instanceCount*/
void FRenderCommandPresent();

#endif