#ifndef __FPACKAGE_H__
#define __FPACKAGE_H__

#include "Core/enTypes.h"
#include "Core/enFile.h"
#include "Core/enArray.h"
#include "Core/enHashMap.h"

#define ENTERPRISE_PACKAGE_LATEST_VERSION 2
#define ENTERPRISE_PACKAGE_MAGIC_NUMBER 0x3DE2920ED93
#define ENTERPRISE_PACKAGE_MAX_RECORDS 65536

typedef enum enRecordType
{
    enRecordType_Remove, /* Used for defragmenting */
    enRecordType_Data, /* Bytes that can be stored as data */
    enRecordType_Audio, /* Must be a .wav file */
    enRecordType_Mesh, /* Must be a .glb file */
    enRecordType_Texture, /* Must be a .png file */
    enRecordType_Font, /* Type undetermined yet */
} enRecordType;

typedef struct enPackageHeader
{
    int64 magicNumber;
    int64 version;
    int64 recordCount; 
} enPackageHeader;

typedef struct enPackageFooter
{
    int64 magicNumber;
    int32 version;
} enPackageFooter;

typedef struct enPackageRecord
{
    int32 recordId; /* A hash of the records name */
    int16 recordType;
    int64 dataCompressedSize; /* 0 if it is not compressed */
    int64 dataSize;
    int64 dataOffset;
} enPackageRecord;

typedef struct enPackage
{
    char recordsPath[FPATH_MAX], dataPath[FPATH_MAX]; 

    enPackageRecord* records;
    enHashMap* recordsMap;
} enPackage;

enPackage* enPackageOpen(const char* pRecordsPath, const char* pDataPath);
void enPackageClose(enPackage* pPackage);
bool enPackageRecordAppend(enPackage* pPackage, const enPackageRecord* pRecord);
bool enPackageRecordMarkRemove(enPackage* pPackage, const char* pNameId);
bool enPackageRecordChange(enPackage* pPackage, const enPackageRecord* pRecord);
bool enPackageDataAppend(enPackage* pPackage, const void* pData, int64 size, int64* pOffset);
const enPackageRecord* enPackageGetRecord(enPackage* pPackage, const char* pName);
bool enPackageDefragment(enPackage* pPackage); /* Removes the marked records and removes them from data in the package file */

#endif