#ifndef __FASSET_H__
#define __FASSET_H__

#include "Core/enTypes.h"

#define PACKAGE_MAGIC_NUMBER 0x454e5452
#define PACKAGE_VERSION_NUMBER 0x0002
#define PACKAGE_ASSET_MAX_COUNT 8192
#define PACKAGE_ASSET_MAX_SIZE (FInt64)ENTERPRISE_MB(100)

typedef enum enAssetType
{
    enAssetType_GltfBinary = 0,
    enAssetType_Wav,
    enAssetType_Max = enAssetType_Wav
} enAssetType;

typedef struct enPackageHeader
{
    int32 magic;
    int32 version;
    int32 count;
} enPackageManifestHeader;

typedef struct enPackageRecord
{
    int32 hash; /* A hash asset name */
    int32 type;
    int32 length; /* If compressed or not compressed this is the actual length */
    int32 uncompressedLength; /* If compressed this is length before compression */
    int32 offset; /* Offset in data file */
} enPackageManifestRecord;

typedef struct enPackageFooter
{
    int64 magic;
    int64 version;
} enPackageFooter;

typedef struct enAsset
{
    const void* pData;
    int64 size;
    enAssetType type;
} enAsset;

#endif