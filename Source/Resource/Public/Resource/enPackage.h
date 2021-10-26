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
    char recordPath[FPATH_MAX];
    char dataPath[FPATH_MAX]; 

    uint32 count;
    enPackageRecord* pRecords;
    int32 hashToRecord[ENTERPRISE_PACKAGE_MAX_RECORDS];
} enPackage;

enPackage* enPackageOpen(const char* pRecordsPath, const char* pDataPath);
void enPackageClose(enPackage* pPackage);
bool enPackageRecordExists(enPackage* pPackage, const char* pName);
bool enPackageAddData(enPackage* pPackage, const char* pName, const enAssetType type, const uint32 length, const uint8* pData); /* Should only be used when developing or patching */
bool enPackageUpdateData(enPackage* pPackage, const char* pName, const enAssetType type, bool updateData, const uint32 length, const uint8* pData); /* Should only be used when patching */
void enPackageRepack(enPackage* pPackage); /* Removes records that are marked remove */
bool enPackagePatch(enPackage* pPackage, const char* pRecordPath, const char* pDataPath); /* Adds new and updates existing records/data */
bool enPackageLoad(enPackage* pPackage, const char* pName, enAsset* pAsset);

#endif