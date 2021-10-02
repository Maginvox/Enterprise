/**
 * @ Author: Your name
 * @ Create Time: 2021-08-15 22:04:11
 * @ Modified by: Your name
 * @ Modified time: 2021-08-18 09:11:51
 * @ Description:
 */

#ifndef __FASSET_H__
#define __FASSET_H__

#include "Core/FTypes.h"

#define PACKAGE_MAGIC_NUMBER (FInt64)0x032AB3AB39
#define PACKAGE_VERSION_NUMBER (FInt64)0x0002
#define PACKAGE_ASSET_MAX_COUNT (FInt64)8192
#define PACKAGE_ASSET_MAX_SIZE (FInt64)F_MB(100)

typedef struct FPackageManifestHeader
{
    int64 versionNumber;
    int64 recordCount;
} FPackageManifestHeader;

typedef struct FPackageManifestRecord
{
    int64 id; /* A hash asset name */
    int64 type;
    int64 compressedSize;
    int64 size;
    int64 dataOffset;
} FPackageManifestRecord;

typedef struct FPackageManifestFooter
{
    int64 magicNumber;
    int64 versionNumber;
} FPackageManifestFooter;


typedef enum EAssetType
{
    E_ASSET_TYPE_GLTF_GLB = 0,
    E_ASSET_TYPE_MAX = E_ASSET_TYPE_GLTF_GLB
} EAssetType;

typedef struct FAsset
{
    void* pData;
    int64 size;
    EAssetType type;
} FAsset;

#endif