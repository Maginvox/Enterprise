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
#define ENTERPRISE_PACKAGE_MAX_RECORDS      65536

typedef struct enPackage
{
    char recordsPath[FPATH_MAX];
    char dataPath[FPATH_MAX]; 

    enPackageRecord* records;
    enHashMap* recordsMap;
} enPackage;

enPackage* enPackageOpen(const char* pRecordsPath, const char* pDataPath);
void enPackageClose(enPackage* pPackage);
bool enPackageRegister(enPackage* pPackage, const enPackageRecord* record);
void enPackageUnRegister(enPackage* pPackage, const uint32 hash);



bool enPackageRecordUpdate(enPackage* pPackage, uint32 hash, const enPackageRecord* record);
bool enPackageDataWrite(enPackage* pPackage, uint32 hash, const void* data);

bool enPackageRepack(enPackage* pPackage);

void enPackageReadAsset(enPackage* pPackage, uint32 hash);

#endif