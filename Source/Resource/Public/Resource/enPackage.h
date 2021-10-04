#ifndef __FPACKAGE_H__
#define __FPACKAGE_H__

#include "Core/enTypes.h"
#include "Core/enFile.h"
#include "Core/enArray.h"
#include "Core/enHashMap.h"

#define ENTERPRISE_PACKAGE_LATEST_VERSION 2
#define ENTERPRISE_PACKAGE_MAGIC_NUMBER 0x3DE2920ED93

typedef enum ERecordType
{
    E_RECORD_TYPE_REMOVE, /* Used for defragmenting */
    E_RECORD_TYPE_DATA, /* Bytes that can be stored as data */
    E_RECORD_TYPE_AUDIO, /* Must be a .wav file */
    E_RECORD_TYPE_MESH, /* Must be a .glb file */
    E_RECORD_TYPE_TEXTURE, /* Must be a .png file */
    E_RECORD_TYPE_FONT, /* Type undetermined yet */
} ERecordType;

typedef struct FPackageHeader
{
    int64 magicNumber;
    int64 version;
    int64 recordCount; 
} FPackageHeader;

typedef struct FPackageFooter
{
    int64 magicNumber;
    int32 version;
} FPackageFooter;

typedef struct FPackageRecord
{
    int32 recordId; /* A hash of the records name */
    int16 recordType;
    int64 dataCompressedSize; /* 0 if it is not compressed */
    int64 dataSize;
    int64 dataOffset;
} FPackageRecord;

typedef struct FPackage
{
    char recordsPath[FPATH_MAX], dataPath[FPATH_MAX]; 

    enArray* pRecords;
    enHashMap* pRecordsMap;
} FPackage;

FPackage* enPackageCreate(const char* pRecordsPath, const char* pDataPath);
void enPackageDestroy(FPackage** ppPackage);
bool enPackageRecordAppend(FPackage* pPackage, const FPackageRecord* pRecord);
bool enPackageRecordMarkRemove(FPackage* pPackage, const char* pNameId);
bool enPackageRecordChange(FPackage* pPackage, const FPackageRecord* pRecord);
bool enPackageDataAppend(FPackage* pPackage, const void* pData, int64 size, int64* pOffset);
const FPackageRecord* enPackageGetRecord(FPackage* pPackage, const char* pName);
bool enPackageDefragment(FPackage* pPackage); /* Removes the marked records and removes them from data in the package file */

#endif