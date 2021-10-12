#ifndef __ENTERPRISE_PACKAGE_H__
#define __ENTERPRISE_PACKAGE_H__

#include "Core/enTypes.h"
#include "Core/enFile.h"
#include "Core/enArray.h"
#include "Core/enHashMap.h"

#include "Resource/enAsset.h"

/* A package is a two file system that allows for easy patches */

#define ENTERPRISE_PACKAGE_LATEST_VERSION   2
#define ENTERPRISE_PACKAGE_MAGIC_NUMBER     0x454E5452
#define ENTERPRISE_PACKAGE_MAX_RECORDS      32768

typedef struct enPackage
{
    char recordsPath[FPATH_MAX];
    char dataPath[FPATH_MAX]; 

    uint32 recordsCount;
    enPackageRecord* records;
    int32 hashToRecordMap[ENTERPRISE_PACKAGE_MAX_RECORDS];
    enAsset* assets[ENTERPRISE_PACKAGE_MAX_RECORDS];
} enPackage;

enPackage* enPackageOpen(const char* pRecordsPath, const char* pDataPath);
void enPackageClose(enPackage* pPackage);
bool enPackageAdd(enPackage* package, const char* name, const enAssetType type, const uint32 length, const void* data); /* Should only be used when developing or patching */
bool enPackageAddFile(enPackage* package, const char* path, const enAssetType type); /* Should only be used when developing or patching */
bool enPackageUpdate(enPackage* package, const enPackageRecord* record, const void* data); /* Should only be used when developing or patching */
bool enPackageUpdateFile(enPackage* package, const char* path); /* Use only in development */
bool enPackageRemove(enPackage* package, const char* name); /* Should only be used when developing or patching */
bool enPackageRepack(enPackage* package); /* Removes any records marked as remove. */
const enAsset* enPackageLoadAsset(enPackage* pPackage, const char* name);
void enPackageUnLoadAsset(enPackage* pPackage, const char* name);
bool enPackagePatch(enPackage* package, const char* patchRecordsPath, const char* patchDataPath);

#endif