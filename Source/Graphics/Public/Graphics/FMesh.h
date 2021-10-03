/* Copyright © 2021 Caden Miller, All Rights Reserved. */

#ifndef __FMESH_H__
#define __FMESH_H__

typedef void* FMesh;

FMesh FMeshCreate(FMeshData* pParsedMesh);
void FMeshDestroy(FMesh mesh);

#endif