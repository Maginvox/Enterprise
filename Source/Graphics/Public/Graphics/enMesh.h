/* Copyright © 2021 Caden Miller, All Rights Reserved. */

#ifndef __FMESH_H__
#define __FMESH_H__

typedef void* FMesh;

FMesh enMeshCreate(FMeshData* pParsedMesh);
void enMeshDestroy(FMesh mesh);

#endif