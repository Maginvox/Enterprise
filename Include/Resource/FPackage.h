#ifndef __FPACKAGE_H__
#define __FPACKAGE_H__

#include "FTypes.h"
#include "Core/FArray.h"
#include "Core/FHashMap.h"

#define FPACKAGE_LATEST_VERSION 2
#define FPACKAGE_MAGIC_NUMBER 0x3DE2920ED93

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
    FInt64 magicNumber;
    FInt64 version;
    FInt64 recordCount; 
} FPackageHeader;

typedef struct FPackageFooter
{
    FInt64 magicNumber;
    FInt32 version;
} FPackageFooter;

typedef struct FPackageRecord
{
    FInt32 recordId; /* A hash of the records name */
    FInt16 recordType;
    FInt64 dataCompressedSize; /* 0 if it is not compressed */
    FInt64 dataSize;
    FInt64 dataOffset;
} FPackageRecord;

typedef struct FPackage
{
    char recordsPath[FPATH_MAX], dataPath[FPATH_MAX]; 

    FArray* pRecords;
    FHashMap* pRecordsMap;
} FPackage;

FPackage* FPackageCreate(const char* pRecordsPath, const char* pDataPath);
void FPackageDestroy(FPackage** ppPackage);
bool FPackageRecordAppend(FPackage* pPackage, const FPackageRecord* pRecord);
bool FPackageRecordMarkRemove(FPackage* pPackage, const char* pNameId);
bool FPackageRecordChange(FPackage* pPackage, const FPackageRecord* pRecord);
bool FPackageDataAppend(FPackage* pPackage, const void* pData, FInt64 size, FInt64* pOffset);
const FPackageRecord* FPackageGetRecord(FPackage* pPackage, const char* pName);
bool FPackageDefragment(FPackage* pPackage); /* Removes the marked records and removes them from data in the package file */

#endif