#ifndef __FASSET_H__
#define __FASSET_H__

#include "Core/enTypes.h"

#define PACKAGE_MAGIC_NUMBER 0x454e5452
#define PACKAGE_VERSION_NUMBER 0x0003
#define PACKAGE_ASSET_MAX_COUNT 8192
#define PACKAGE_ASSET_MAX_SIZE (FInt64)ENTERPRISE_MB(100)

typedef enum enAssetType
{
    enAssetType_Remove = 0,
    enAssetType_None,
    enAssetType_GltfBinary = 0,
    enAssetType_Wav,
    enAssetType_Max = enAssetType_Wav
} enAssetType;

typedef struct enPackageHeader
{
    uint32 magic;
    uint32 version;
    uint32 count;
} enPackageHeader;

typedef struct enPackageRecord
{
    uint32 hash; /* A hash asset name */
    char md5[16]; /* Data md5 hash */
    uint32 type;
    uint32 length; /* If compressed or not compressed this is the actual length */
    uint32 uncompressedLength; /* If compressed this is length before compression */
    uint32 offset; /* Offset in data file */
} enPackageRecord;

typedef struct enPackageFooter
{
    uint64 magic;
    uint64 version;
} enPackageFooter;

typedef struct enAsset
{
    void* data; /* A running program should never change this data only read */
    uint64 size;
    enAssetType type;
} enAsset;

#endif