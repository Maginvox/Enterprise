/* Copyright © 2021 Caden Miller, All Rights Reserved. */

#include "lz4.h"

#include "Core/enMemory.h"
#include "Core/enMath.h"
#include "Core/enHash.h"
#include "Core/enLog.h"
#include "Core/enFile.h"

#include "Resource/enPackage.h"

bool enPackageRewrite(enPackage* package) /* Rewrites currently loaded records into the file */
{
    enFile* recordsFile = enFileOpen(package->recordsPath, "w");
    if (!recordsFile)
    {
        return false;
    }

    /* Write the header */
    enPackageHeader header = {
        .magic = ENTERPRISE_PACKAGE_MAGIC_NUMBER,
        .version = ENTERPRISE_PACKAGE_LATEST_VERSION,
        .count = package->recordsCount,
    };

    if (enFileWrite(recordsFile, &header, sizeof(enPackageHeader), 1) != 1 ||
        enFileWrite(recordsFile, package->records, sizeof(enPackageRecord), package->recordsCount) != package->recordsCount)
    {
        enFileClose(recordsFile);
        return false;
    }

    enFileClose(recordsFile);
    return true;
}

enPackage* enPackageOpen(const char* recordPath, const char* packagePath)
{
    if (recordPath == NULL || packagePath == NULL)
    {
        return NULL;
    }

    /* Create or open the files */
    enFile* recordFile = enFileOpenOrCreate(recordPath, "r");
    if (!recordFile)
    {
        return NULL;
    }
     
    enFile* packageFile = enFileOpenOrCreate(packagePath, "r");
    if (!packageFile)
    {
        enFileClose(recordFile);
        return NULL;
    }

    enFileClose(packageFile); /* We don't need this file right now. We just have to make sure it can exist for later */

    /* Allocate the package */
    enPackage* package = enCalloc(1, sizeof(enPackage));
    if (!package)
    {
        return NULL;
    }

    enStringCopy(recordPath, FPATH_MAX, package->recordsPath, FPATH_MAX);
    enStringCopy(packagePath, FPATH_MAX, package->dataPath, FPATH_MAX);
    enMemorySet(package->hashToRecordMap, sizeof(package->hashToRecordMap), -1);

    /* Read the record header */
    enPackageHeader header = {0};
    if (!enFileRead(recordFile, &header, sizeof(enPackageHeader), 1))
    {
        enFileClose(recordFile); /* We must close the file first before executing a function that will reopen it */
        if (!enPackageRewrite(package))
        {
            enFileClose(recordFile);
            enPackageClose(package);
            enLogError("Could not write default header to package file!");
            return NULL;
        }
        else
        {
            recordFile = enFileOpen(recordPath, "r");
            header = (enPackageHeader){
                .magic = ENTERPRISE_PACKAGE_MAGIC_NUMBER,
                .version = ENTERPRISE_PACKAGE_LATEST_VERSION,
                .count = 0,
            };
        }
    }

    /* Check the magic and version */
    if (header.magic != ENTERPRISE_PACKAGE_MAGIC_NUMBER || header.version != ENTERPRISE_PACKAGE_LATEST_VERSION || !enMathIsBetween(header.count, 0, ENTERPRISE_PACKAGE_MAX_RECORDS))
    {
        enFileClose(recordFile);
        enPackageClose(package);
        return NULL;
    }

    package->recordsCount = header.count;

    /* Read the records */
    package->records = enCalloc(package->recordsCount, sizeof(enPackageRecord));
    if (!package->records)
    {
        enFileClose(recordFile);
        enPackageClose(package);
        return NULL;
    }

    if (enFileRead(recordFile, package->records, sizeof(enPackageRecord), package->recordsCount) != package->recordsCount)
    {
        enFileClose(recordFile);
        enPackageClose(package);
        enLogError("Could not read package records!");
        return NULL;
    }

    enFileClose(recordFile);

    /* Map the record hashes to indices */
    for (uint32 i = 0; i < package->recordsCount; i++)
    {
        enPackageRecord* record = &package->records[i];

        if (record->hash > ENTERPRISE_PACKAGE_MAX_RECORDS)
        {
            continue; /* The record is malformed. */
        }

        package->hashToRecordMap[record->hash] = i;
    }

    return package;
}

void enPackageClose(enPackage* package)
{
    if (!package)
    {
        return;
    }

    if (package->records != NULL)
    {
        enFree(package->records);
    }
    enFree(package);
}

bool enPackageAdd(enPackage* package, const char* name, const enAssetType type, const uint32 length, const void* data)
{

    if (package == NULL || !enMathIsBetween(type, 0, enAssetType_Max) || length == 0 || data == NULL)
    {
        return false;        
    }

    uint32 hash = enHashMultiplicationMethod(name);
    hash %= ENTERPRISE_PACKAGE_MAX_RECORDS;

    /* Make sure that the record does not exist */
    if (hash > ENTERPRISE_PACKAGE_MAX_RECORDS || package->hashToRecordMap[hash] != -1)
    {
        return false;
    }

    enPackageRecord record = {
        .hash = hash,
        .type = type,
        .uncompressedLength = length
    };

    /* Compress the data if possible */
    uint32 compressedSize = LZ4_compressBound(length);
    uint8* compressedBytes = enMalloc(compressedSize);
    if (!compressedBytes)
    {
        return false;
    }

    bool compressed = true;
    compressedSize = LZ4_compress_default(data, compressedBytes, length, compressedSize);
    if (compressedSize <= 0)
    {
        enFree(compressedBytes);
        enLogError("Could not compress an asset!");
        return false;
    }

    if (compressedSize >= length)
    {
        enLogWarning("Could not compress an asset, its size is greater than when not compressed!");
        compressed = false;
    }

    record.length = compressedSize;

    enFile* pDataFile = enFileOpen(package->dataPath, "a");
    if (!pDataFile)
    {
        return false;
    }

    /* Write the data to the file */
    record.offset = (uint32)enFileTell(pDataFile);
    if (enFileWrite(pDataFile, (compressed) ? compressedBytes : data, record.length, 1) != 1)
    {
        enFileClose(pDataFile);
        return false;
    }

    enFree(compressedBytes);
    enFileClose(pDataFile);


    /* Write the record to the record file */
    enFile* recordFile = enFileOpen(package->recordsPath, "a");
    if (!recordFile)
    {
        return false;
    }

    if (enFileWrite(recordFile, &record, sizeof(enPackageRecord), 1) != 1)
    {
        enFileClose(recordFile);
        return false;
    }

    enFileClose(recordFile);

    /* Update the package */
    package->recordsCount++;
    package->records = enRealloc(package->records, sizeof(enPackageRecord) * package->recordsCount);
    package->records[package->recordsCount - 1] = record;
    package->hashToRecordMap[hash] = package->recordsCount - 1;
    
    return true;
}

bool enPackageAddFile(enPackage* package, const char* path, const enAssetType type)
{
    if (package == NULL || path == NULL || !enMathIsBetween(type, 0, enAssetType_Max))
    {
        return false;
    }

    enFile* file = enFileOpen(path, "rb");
    if (!file)
    {
        return false;
    }

    /* Get the file size */
    enFileSeek(file, 0, enFileSeek_End);
    uint32 length = (uint32)enFileTell(file);
    enFileSeek(file, 0, enFileSeek_Set);

    /* Read the file data */
    uint8* data = enMalloc(length);
    if (!data)
    {
        enFileClose(file);
        return false;
    }

    if (enFileRead(file, data, length, 1) != 1)
    {
        enFileClose(file);
        enFree(data);
        return false;
    }

    enFileClose(file);

    /* Add the asset */
    return enPackageAdd(package, path, type, length, data);
}

bool enPackageRemove(enPackage* package, const char* name)
{
    if (package == NULL)
    {
        return false;
    }

    uint32 hash = enHashMultiplicationMethod(name);
    hash %= ENTERPRISE_PACKAGE_MAX_RECORDS;

    if (hash > ENTERPRISE_PACKAGE_MAX_RECORDS || package->hashToRecordMap[hash] == -1)
    {
        return false;
    }

    enPackageRecord* record = &package->records[package->hashToRecordMap[hash]];
    record->type = enAssetType_Remove;

    /* Record this in the record file */
    enFile* recordFile = enFileOpen(package->recordsPath, "r+");
    if (!recordFile)
    {
        return false;
    }

    uint32 offset = sizeof(enPackageHeader) + (package->hashToRecordMap[hash] * sizeof(enPackageRecord));

    if (enFileSeek(recordFile, offset, enFileSeek_Set) != record->offset)
    {
        enFileClose(recordFile);
        return false;
    }

    if (enFileWrite(recordFile, record, sizeof(enPackageRecord), 1) != 1)
    {
        enFileClose(recordFile);
        return false;
    }

    enFileClose(recordFile);
    return true;
}

