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
    if (recordsFile == NULL)
    {
        return false;
    }

    /* Write the header */
    const enPackageHeader header = {
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
        enFileClose(recordFile); /* enPackageRewrite opens the record file again so we must close it here */

        if (!enPackageRewrite(package))
        {
            enPackageClose(package);
            enLogError("Could not write default header to package file!");
            return NULL;
        }
        else
        {
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

    if (header.count > 0)
    {
    
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
    }

    /* Map the record hashes to indices */
    for (uint32 i = 0; i < package->recordsCount; i++)
    {
        enPackageRecord* record = &package->records[i];

        if (record->hash > ENTERPRISE_PACKAGE_MAX_RECORDS)
        {
            enLogError("A package record is malformed!");
            enPackageClose(package);
            return NULL; /* The record is malformed. */
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
        enLogInfo("Could not compress an asset, its size is greater than when not compressed!");
        compressed = false;
    }

    /* Add the record */
    int32 offset = enPackageDataAppend(package, compressedSize, compressedBytes);
    if (offset == -1)
    {
        enFree(compressedBytes);
        enLogError("Could not append data to package!");
        return false;
    }

    enFree(compressedBytes);

    const enPackageRecord record = {
        .hash = hash,
        .type = type,
        .length = (compressed ? compressedSize : length),
        .uncompressedLength = length,
        .offset = offset
    };

    /* Add the record to the package */
    package->recordsCount++;
    enPackageRecord* newRecords = enRealloc(package->records, sizeof(enPackageRecord) * package->recordsCount);
    if (newRecords == NULL)
    {
        enLogError("Could not reallocate package records!");
        return false;
    }

    package->records = newRecords;
    package->records[package->recordsCount - 1] = record;
    package->hashToRecordMap[hash] = package->recordsCount - 1;

    return enPackageRewrite(package);
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
    enFileSeek(file, 0, enSeek_End);
    uint32 length = (uint32)enFileTell(file);
    enFileSeek(file, 0, enSeek_Set);

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
    if (!enPackageAdd(package, path, type, length, data))
    {
        enFree(data);
        return false;
    }

    enFree(data);
    return true;
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
    return enPackageRewrite(package);
}

bool enPackageRepack(enPackage* package)
{

    /* Count the number of records that will be left over */
    uint32 recordsLeft = 0;
    for (uint32 i = 0; i < package->recordsCount; i++)
    {
        if (package->records[i].type != enAssetType_Remove)
        {
            recordsLeft++;
        }
    }

    /* Allocate the new records */
    enPackageRecord* newRecords = enMalloc(sizeof(enPackageRecord) * recordsLeft);
    if (newRecords == NULL)
    {
        return false;
    }

    /* Copy the records */
    uint32 newRecordsIndex = 0;
    for (uint32 i = 0; i < package->recordsCount; i++)
    {
        if (package->records[i].type != enAssetType_Remove)
        {
            newRecords[newRecordsIndex] = package->records[i];
            newRecordsIndex++;
        }
    }

    enFree(package->records);
    package->records = newRecords;
    package->recordsCount = recordsLeft;

    /* Open the data file */
    enFile* dataFile = enFileOpen(package->dataPath, "r+");
    if (dataFile == NULL)
    {
        return false;
    }

    /* Create a copy of the data file */
    char copyPath[256] = {0};
    enStringCopy(package->dataPath, 256, copyPath, 256);
    enStringCopy(".temp", sizeof(".temp"), copyPath, 256);

    enFile* copyFile = enFileOpen(package->dataPath, "w");
    if (copyFile == NULL)
    {
        enFileClose(dataFile);
        return false;
    }
    
    for (uint32 i = 0; i < package->recordsCount; i++)
    {
        enPackageRecord* record = &package->records[i];

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
                    return false;
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
        return false;
    }

    enFileClose(dataFile);
    enFileClose(copyFile);

    /* Delete the data file and rename the copy */
    enFileRemove(package->dataPath);
    enFileRename(copyPath, package->dataPath);
    
    return true;
}

