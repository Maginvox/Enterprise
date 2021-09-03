/**
 * @ Author: Your name
 * @ Create Time: 2021-08-15 22:04:11
 * @ Modified by: Your name
 * @ Modified time: 2021-08-18 09:11:51
 * @ Description:
 */

#ifndef __FASSET_H__
#define __FASSET_H__

#include "FTypes.h"

#define PACKAGE_MAGIC_NUMBER (FInt64)0x032AB3AB39
#define PACKAGE_VERSION_NUMBER (FInt64)0x0002
#define PACKAGE_ASSET_MAX_COUNT (FInt64)8192
#define PACKAGE_ASSET_MAX_SIZE (FInt64)F_MB(100)

typedef struct FPackageManifestHeader
{
    FInt64 versionNumber;
    FInt64 recordCount;
} FPackageManifestHeader;

typedef struct FPackageManifestRecord
{
    FInt64 id; /* A hash asset name */
    FInt64 type;
    FInt64 compressedSize;
    FInt64 size;
    FInt64 dataOffset;
} FPackageManifestRecord;

typedef struct FPackageManifestFooter
{
    FInt64 magicNumber;
    FInt64 versionNumber;
} FPackageManifestFooter;


typedef enum EAssetType
{
    E_ASSET_TYPE_GLTF_GLB = 0,
    E_ASSET_TYPE_MAX = E_ASSET_TYPE_GLTF_GLB
} EAssetType;

typedef struct FAsset
{
    void* pData;
    FInt64 size;
    EAssetType type;
} FAsset;

#endif