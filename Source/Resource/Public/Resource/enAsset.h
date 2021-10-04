#ifndef __FASSET_H__
#define __FASSET_H__

#include "Core/enTypes.h"

#define PACKAGE_MAGIC_NUMBER (FInt64)0x032AB3AB39
#define PACKAGE_VERSION_NUMBER 0x0002
#define PACKAGE_ASSET_MAX_COUNT 8192
#define PACKAGE_ASSET_MAX_SIZE (FInt64)ENTERPRISE_MB(100)

typedef struct
{
    int64 versionNumber;
    int64 recordCount;
} enPackageManifestHeader;

typedef struct
{
    int64 id; /* A hash asset name */
    int64 type;
    int64 compressedSize;
    int64 size;
    int64 dataOffset;
} enPackageManifestRecord;

typedef struct
{
    int64 magicNumber;
    int64 versionNumber;
} enPackageManifestFooter;

typedef enum
{
    enAssetType_GLB = 0,
    enAssetType_Max = enAssetType_GLB
} enAssetType;

typedef struct
{
    void* pData;
    int64 size;
    enAssetType type;
} enAsset;

#endif