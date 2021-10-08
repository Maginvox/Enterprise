#ifndef __FPACKAGE_H__
#define __FPACKAGE_H__

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

    enArray* records;
    enAsset* loadedAssets;
    int32 hashToRecordMap[ENTERPRISE_PACKAGE_MAX_RECORDS];
} enPackage;

enPackage* enPackageOpen(const char* pRecordsPath, const char* pDataPath);
void enPackageClose(enPackage* pPackage);
void enPackageAdd(enPackage* package, const enPackageRecord* record, const void* data);
void enPackageRemove(enPackage* package, uint32 hash);
void enPackageRepack(enPackage* package); /* Removes any records marked as remove. */
void enPackageReadAsset(enPackage* pPackage, uint32 hash);

#endif