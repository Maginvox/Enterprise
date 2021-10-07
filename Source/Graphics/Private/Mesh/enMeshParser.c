#include "Core/enMemory.h"
#include "Core/enLog.h"
#include "Core/enJsonParser.h"

#include "Graphics/enMeshParser.h"

#include "enMeshParserGLTF.h"

enMesh* enMeshParse(enAsset* pAsset)
{
    enMesh* pMesh = enCalloc(1, sizeof(enMesh));

    switch (pAsset->type)
    {
        case enAssetType_GltfBinary:
            if (!enMeshParserGLTF(pAsset, pMesh))
            {
                return NULL;
            }
            break;
    
        default:
            enLogError("Asset was not of mesh type!");
            break;
    }

    return pMesh;
}