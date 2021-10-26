/* Copyright © 2021 Caden Miller, All Rights Reserved. */

#include "lz4.h"

#include "Core/enMemory.h"
#include "Core/enMath.h"
#include "Core/enHash.h"
#include "Core/enMD5.h"
#include "Core/enLog.h"
#include "Core/enFile.h"

#include "Resource/enPackage.h"

static inline uint32 enPackageHash(const char* pName) 
{
    return enHashMultiplicationMethod(pName) % ENTERPRISE_PACKAGE_MAX_RECORDS;
}

bool enPackageRewrite(enPackage* package) /* Rewrites currently loaded records into the file */
{
    enFile* recordsFile = enFileOpen(package->recordPath, "w");
    if (recordsFile == NULL)
    {
        return false;
    }

    /* Write the header */
    const enPackageHeader header = {
        .magic = ENTERPRISE_PACKAGE_MAGIC_NUMBER,
        .version = ENTERPRISE_PACKAGE_LATEST_VERSION,
        .count = package->count,
    };

    if (enFileWrite(recordsFile, &header, sizeof(enPackageHeader), 1) != 1 ||
        enFileWrite(recordsFile, package->pRecords, sizeof(enPackageRecord), package->count) != package->count)
    {
        enFileClose(recordsFile);
        return false;
    }

    enFileClose(recordsFile);
    return true;
}

int32 enPackageDataAppend(enPackage* package, uint32 length, void* data) /* Returns offset, if -1 error occured */
{
    if (package == NULL || length == 0 || data == NULL)
    {
        return -1;
    }

    /* Open in append mode */
    enFile* dataFile = enFileOpen(package->dataPath, "a");
    if (dataFile == NULL)
    {
        return -1;
    }

    /* Get our offset and write the data */
    int32 offset = (int32)enFileTell(dataFile);
    if (enFileWrite(dataFile, data, length, 1) != 1)
    {
        enFileClose(dataFile);
        return -1;
    }

    enFileClose(dataFile);
    return offset;
}

enPackage* enPackageOpen(const char* recordPath, const char* packagePath)
{
    if (recordPath == NULL || packagePath == NULL)
    {
        return NULL;
    }

    /* Create or open the files */
    enFile* recordFile = enFileOpenOrCreate(recordPath, "r+");
    if (!recordFile)
    {
        return NULL;
    }

    /* Allocate the package */
    enPackage* pPackage = enCalloc(1, sizeof(enPackage));
    if (!pPackage)
    {
        return NULL;
    }

    enStringCopy(recordPath, FPATH_MAX, pPackage->recordPath, FPATH_MAX);
    enStringCopy(packagePath, FPATH_MAX, pPackage->dataPath, FPATH_MAX);
    enMemorySet(pPackage->hashToRecord, sizeof(pPackage->hashToRecord), -1);

    /* Read the record header */
    enPackageHeader header = {0};
    if (!enFileRead(recordFile, &header, sizeof(enPackageHeader), 1))
    {
        enFileClose(recordFile); /* enPackageRewrite opens the record file again so we must close it here */

        if (!enPackageRewrite(pPackage))
        {
            enPackageClose(pPackage);
            enLogError("Could not write default header to package file!");
            return NULL;
        }
        else /* There is no records since we just created the package, we can return */
        {
            return pPackage;
        }
    }

    /* Check the magic and version */
    if (header.magic != ENTERPRISE_PACKAGE_MAGIC_NUMBER || header.version != ENTERPRISE_PACKAGE_LATEST_VERSION || !enMathIsBetween(header.count, 0, ENTERPRISE_PACKAGE_MAX_RECORDS))
    {
        enFileClose(recordFile);
        enPackageClose(pPackage);
        enLogError("Could not read package magic and version properly!");
        return NULL;
    }

    pPackage->count = header.count;

    if (header.count > 0)
    {  
        /* Read the records */
        pPackage->pRecords = enCalloc(pPackage->count, sizeof(enPackageRecord));
        if (!pPackage->pRecords)
        {
            enFileClose(recordFile);
            enPackageClose(pPackage);
            return NULL;
        }

        if (enFileRead(recordFile, pPackage->pRecords, sizeof(enPackageRecord), pPackage->count) != pPackage->count)
        {
            enFileClose(recordFile);
            enPackageClose(pPackage);
            enLogError("Could not read package records!");
            return NULL;
        }
   }

    enFileClose(recordFile);

    /* Map the record hashes to indices */
    for (uint32 i = 0; i < pPackage->count; i++)
    {
        enPackageRecord* record = &pPackage->pRecords[i];
        if (record->hash > ENTERPRISE_PACKAGE_MAX_RECORDS)
        {
            enLogError("A package record is malformed!");
            enPackageClose(pPackage);
            return NULL; /* The record is malformed. */
        }

        pPackage->hashToRecord[record->hash] = i;
    }

    return pPackage;
}

void enPackageClose(enPackage* package)
{
    if (!package)
    {
        return;
    }

    /* Free the records */
    if (package->pRecords != NULL)
    {
        enFree(package->pRecords);
    }

    enFree(package);
}

bool enPackageRecordExists(enPackage* pPackage, const char* pName)
{
    if (!pPackage || !pName)
    {
        return false;
    }

    uint32 hash = enPackageHash(pName);
    return pPackage->hashToRecord[hash] != -1;
}

bool enPackageDataAdd(enPackage* pPackage, const char* pName, const enAssetType type, const uint32 length, const uint8* pData)
{

    if (pPackage == NULL || !enMathIsBetween(type, 0, enAssetType_Max) || length == 0 || pData == NULL)
    {
        return false;        
    }

    uint32 hash = enPackageHash(pName);

    /* Make sure that the record does not exist */
    if (pPackage->hashToRecord[hash] != -1)
    {
        return false;
    }

    /* Compress the data if possible */
    uint32 compressedSize = LZ4_compressBound(length);
    uint8* compressedBytes = enMalloc(compressedSize);
    if (!compressedBytes)
    {
        return false;
    }

    bool compressed = true;
    compressedSize = LZ4_compress_default(pData, compressedBytes, length, compressedSize);
    if (compressedSize <= 0)
    {
        enFree(compressedBytes);
        enLogError("Could not compress an asset!");
        return false;
    }

    if (compressedSize >= length)
    {
        enLogInfo("Could not compress an asset, its size is greater than when not compressed!");
        compressed = false;
    }

    /* Add the record */
    int32 offset = enPackageDataAppend(pPackage, compressedSize, compressedBytes);
    if (offset == -1)
    {
        enFree(compressedBytes);
        enLogError("Could not append data to package!");
        return false;
    }

    uint8 compressedMD5[16];
    enMD5String(compressedBytes, compressedSize, compressedMD5);

    enFree(compressedBytes);

    enPackageRecord record = {
        .hash = hash,
        .type = type,
        .length = (compressed ? compressedSize : length),
        .uncompressedLength = length,
        .offset = offset
    };

    enMemoryCopy(record.md5, compressedMD5, 16);

    /* Add the record to the package */
    pPackage->count++;
    enPackageRecord* pNewRecords = enRealloc(pPackage->pRecords, sizeof(enPackageRecord) * pPackage->count);
    if (pNewRecords == NULL)
    {
        enLogError("Could not reallocate package records!");
        return false;
    }

    pPackage->pRecords = pNewRecords;
    pPackage->pRecords[pPackage->count - 1] = record;
    pPackage->hashToRecord[hash] = pPackage->count - 1;

    return enPackageRewrite(pPackage);
}

void enPackageRepack(enPackage* package)
{

    /* Count the number of records that will be left over */
    uint32 recordsLeft = 0;
    for (uint32 i = 0; i < package->count; i++)
    {
        if (package->pRecords[i].type != enAssetType_Remove)
        {
            recordsLeft++;
        }
    }

    /* Allocate the new records */
    enPackageRecord* newRecords = enMalloc(sizeof(enPackageRecord) * recordsLeft);
    if (newRecords == NULL)
    {
        return;
    }

    /* Copy the records */
    uint32 newRecordsIndex = 0;
    for (uint32 i = 0; i < package->count; i++)
    {
        if (package->pRecords[i].type != enAssetType_Remove)
        {
            newRecords[newRecordsIndex] = package->pRecords[i];
            newRecordsIndex++;
        }
    }

    enFree(package->pRecords);
    package->pRecords = newRecords;
    package->count = recordsLeft;

    /* Open the data file */
    enFile* dataFile = enFileOpen(package->dataPath, "r+");
    if (dataFile == NULL)
    {
        return;
    }

    /* Create a copy of the data file */
    char copyPath[256] = {0};
    enStringCopy(package->dataPath, 256, copyPath, 256);
    enStringCopy(".temp", sizeof(".temp"), copyPath, 256);

    enFile* copyFile = enFileOpen(package->dataPath, "w");
    if (copyFile == NULL)
    {
        enFileClose(dataFile);
        return;
    }
    
    for (uint32 i = 0; i < package->count; i++)
    {
        enPackageRecord* record = &package->pRecords[i];

        if (record->type != enAssetType_Remove)
        {
            enFileSeek(dataFile, record->offset, enSeek_Set);

            /* Update the record offset */
            record->offset = (uint32)enFileTell(copyFile);

            /* Copy the record data to the new file */
            char c = 0;
            for (uint32 j = 0; j < record->length; j++)
            {
                if ((c = enFileReadChar(dataFile)) == FEOF)
                {
                    enFileClose(dataFile);
                    enFileClose(copyFile);
                    return;
                }

                enFileWriteChar(copyFile, c);
            }
        }
    }

    /* Rewrite the records, with new offsets and removed records */
    if (!enPackageRewrite(package))
    {
        enFileClose(dataFile);
        enFileClose(copyFile);
        return;
    }

    enFileClose(dataFile);
    enFileClose(copyFile);

    /* Delete the data file and rename the copy */
    enFileRemove(package->dataPath);
    enFileRename(copyPath, package->dataPath);
    
    return;
}

bool enPackageLoad(enPackage* package, const char* pName, enAsset* pAsset)
{
    if (!package || !pName || !pAsset)
    {
        return false;
    }

    /* Check if the record exists */
    uint32 hash = enPackageHash(pName);
    if (package->hashToRecord[hash] == -1)
    {
        return false; /* This record does not exist */
    }

    /* Check if the record is to be removed */
    enPackageRecord* record = &package->pRecords[package->hashToRecord[hash]];
    if (record->type == enAssetType_Remove)
    {
        return false; /* Record will not be loaded since it will be deleted soon */
    }

    /* Open the data file */
    enFile* pDataFile = enFileOpen(package->dataPath, "rb");
    if (pDataFile == NULL)
    {
        return false;
    }

    enFileSeek(pDataFile, record->offset, enSeek_Set);

    /* Load the asset data */
    pAsset->pData = enMalloc(record->length);
    pAsset->type = record->type;
    pAsset->size = record->length;

    if (!pAsset->pData)
    {
        enFileClose(pDataFile);
        return false;
    }

    if (enFileRead(pDataFile, pAsset->pData, record->length, 1) != 1)
    {
        enFileClose(pDataFile);
        enFree(pAsset->pData);
        return false;
    }

    enFileClose(pDataFile);
    return true;
}

bool enPackageUpdate(enPackage* package, const char* pName, const enAssetType type, bool updateData, const uint32 length, const void* pData)
{
    if (package == NULL || pName == NULL || !enMathIsBetween(type, 0, enAssetType_Max) || pData == NULL)
    {
        return false;
    }

    /* Make sure that the record does exist */
    uint32 hash = enPackageHash(pName);

    if (package->hashToRecord[hash] == -1)
    {
        return false;
    }

    enPackageRecord* record = &package->pRecords[package->hashToRecord[hash]];

    /* Compress the data if possible */
    uint32 compressedSize = LZ4_compressBound(length);
    uint8* compressedBytes = enMalloc(compressedSize);
    if (!compressedBytes)
    {
        return false;
    }

    bool compressed = true;
    compressedSize = LZ4_compress_default(pData, compressedBytes, length, compressedSize);
    if (compressedSize <= 0)
    {
        enFree(compressedBytes);
        enLogError("Could not compress an asset!");
        return false;
    }
    
    /* If the compressed length larger, then we should just use the normal data */
    if (compressedSize >= length)
    {
        enLogInfo("Could not compress an asset, its size is greater than when not compressed!");
        compressed = false;
    }

    char compressedMD5[16];
    enMD5String(compressedBytes, compressedSize, compressedMD5);

    /* Compare MD5 values */
    if (enStringCompare(compressedMD5, record->md5, 16) == 0 && record->type == type)
    {
        enFree(compressedBytes);
        return true; /* We already have this record so its success */
    }

    /* The values are different so we must upload new data to the package */
    int32 offset = enPackageDataAppend(package, compressedSize, compressedBytes);
    if (offset < 0)
    {
        enFree(compressedBytes);
        enLogError("Could not append data to package!");
        return false;
    }

    /* Set the new record data */
    *record = (enPackageRecord){
        .hash = hash,
        .type = type,
        .length = (compressed ? compressedSize : length),
        .uncompressedLength = length,
        .offset = offset
    };

    enMemoryCopy(record->md5, compressedMD5, 16);

    /* Update the records */
    return enPackageRewrite(package);
}