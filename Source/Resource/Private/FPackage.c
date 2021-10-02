*/* Copyright © 2021 Caden Miller, All Rights Reserved. */

#include "Core/FFile.h"
#include "Core/FMemory.h"
#include "Core/FHash.h"
#include "Core/FLog.h"
#include "Resource/FPackage.h"

bool FPackageRewriteRecords(const FPackage* pPackage)
{
    if (pPackage == NULL)
    {
        return false;
    }

    FFile* pRecordsFile = FFileOpen(pPackage->recordsPath, "w");
    if (pRecordsFile == NULL)
    {
        return false;
    }

    /* Write the header */
    const FPackageHeader header = {
        FPACKAGE_MAGIC_NUMBER,
        FPACKAGE_LATEST_VERSION,
        pPackage->pRecords->count
    };

    if (FFileWrite(pRecordsFile, &header, sizeof(FPackageHeader), sizeof(FPackageHeader), 1) != 1)
    {
        FFileClose(&pRecordsFile);
        return false;
    }

    /* Write all of the records */
    FPackageRecord* pRecords = FArrayData(pPackage->pRecords);
    int32 recordsCount = FArrayCount(pPackage->pRecords);

    if (pRecords != NULL && recordsCount != 0)
    {
        if (FFileWrite(pRecordsFile, pRecords, sizeof(FPackageRecord) * recordsCount, sizeof(FPackageRecord), recordsCount) != recordsCount)
        {
            FFileClose(&pRecordsFile);
            return false;
        }
    }

    /* Write the footer */
    const FPackageFooter footer = {
        FPACKAGE_MAGIC_NUMBER,
        FPACKAGE_LATEST_VERSION
    };

    if (FFileWrite(pRecordsFile, &footer, sizeof(FPackageFooter), sizeof(FPackageFooter), 1) != 1)
    {
        FFileClose(&pRecordsFile);
        return false;
    }

    FFileClose(&pRecordsFile);
    return true;
}

bool FPackageRemapRecords(FPackage* pPackage)
{

    if (pPackage == NULL)
    {
        return false;
    }

    int32 recordsCount = FArrayCount(pPackage->pRecords);
    for (int32 i = 0; i < recordsCount; i++)
    {
        FPackageRecord* pRecord = FArrayGet(pPackage->pRecords, i);
        if (!FHashMapInsertWithHashedKey(pPackage->pRecordsMap, pRecord->recordId, pRecord))
        {
            return false;
        }
    }

    return true;
}

/* ====================================================== */
FPackage* FPackageCreate(const char* pRecordsPath, const char* pDataPath)
{
    if (pRecordsPath == NULL || pDataPath == NULL)
    {
        return NULL;
    }


    FPackage* pPackage = FAllocateZero(1, sizeof(FPackage));
    if (pPackage == NULL)
    {
        return NULL;
    }

    /* Open the records file */
    FFile* pRecordsFile = FFileOpenOrCreate(pRecordsPath, "r+");
    if (pRecordsFile == NULL)
    {
        return NULL;
    }

    /* Read for the header */
    FPackageHeader header = {0};
    if (FFileRead(pRecordsFile, &header, sizeof(FPackageHeader), sizeof(FPackageHeader), 1) != 1)
    {
        /* Generate a default package */
        if (!FPackageRewriteRecords(pPackage))
        {
            FPackageDestroy(&pPackage);
            return NULL;
        }
    }

    return pPackage;
}

/* ====================================================== */
void FPackageDestroy(FPackage** ppPackage)
{
    if (ppPackage == NULL || *ppPackage == NULL)
    {
        return;
    }

    FPackage* pPackage = *ppPackage;

    if (pPackage->pRecords != NULL)
    {
        FArrayDestroy(&pPackage->pRecords);
    }

    if (pPackage->pRecordsMap != NULL)
    {
        FHashMapDestroy(&pPackage->pRecordsMap);
    }

    FDeallocate(pPackage);
}

/* ====================================================== */
bool FPackageRecordAppend(FPackage* pPackage, const FPackageRecord* pRecord)
{
    if (pPackage == NULL || pRecord == NULL)
    {
        return false;
    }

    /* Add the record to the array */
    FArrayAdd(pPackage->pRecords, pRecord);
    FHashMapInsertWithHashedKey(pPackage->pRecordsMap, pRecord->recordId, (void*)pRecord);


    /* Rewrite the records */
    return FPackageRewriteRecords(pPackage);
}

const FPackageRecord* FPackageGetRecord(FPackage* pPackage, const char* pName)
{
    if (pPackage == NULL || pName == NULL)
    {
        return NULL;
    }

    return FHashMapGet(pPackage->pRecordsMap, pName);
}

/* ====================================================== */
bool FPackageRecordMarkRemove(FPackage* pPackage, const char* pNameId)
{
    if (pPackage == NULL || pNameId == NULL)
    {
        return false;
    }

    FPackageRecord* pRecord = FHashMapGet(pPackage->pRecordsMap, pNameId);
    if (pRecord == NULL)
    {
        return false;
    }

    pRecord->recordType = E_RECORD_TYPE_REMOVE;

    /* Open the records file and write the record */
    return FPackageRecordChange(pPackage, pRecord);
}

/* ====================================================== */
bool FPackageRecordChange(FPackage* pPackage, const FPackageRecord* pNewRecord)
{
    if (pPackage == NULL || pNewRecord == NULL)
    {
        return false;
    }

    /* Copy the new record to the old record */
    FPackageRecord* pRecord = (FPackageRecord*)FHashMapGetWithHashedKey(pPackage->pRecordsMap, pNewRecord->recordId);
    if (pRecord == NULL)
    {
        return false;
    }

    FMemoryCopy(pNewRecord, pRecord, sizeof(FPackageRecord));
 
    /* Rewrite the records */
    return FPackageRewriteRecords(pPackage);
}

/* ====================================================== */
bool FPackageDataAppend(FPackage* pPackage, const void* pData, int64 size, int64* pOffset)
{
    if (pPackage == NULL || pData == NULL || size < 1 || pOffset == NULL)
    {
        return false;
    }

    FFile* pDataFile = FFileOpen(pPackage->dataPath, "a");
    if (pDataFile == NULL)
    {
        return false;
    }

    *pOffset = FFileTell(pDataFile);

    if (FFileWrite(pDataFile, pData, size, size, 1) != 1)
    {
        FFileClose(&pDataFile);
        return false;
    }

    FFileClose(&pDataFile);
    return true;
}

/* ====================================================== */
bool FPackageDefragment(FPackage* pPackage)
{
    if (pPackage == NULL)
    {
        return false;
    }

    FFile* pDataFile = FFileOpen(pPackage->dataPath, "r+");
    if (pDataFile == NULL)
    {
        return false;
    }

    /* Open a temporary data file */
    char pTempDataFilePath[FPATH_MAX] = {0};
    FStringCopy(pPackage->dataPath, FPATH_MAX, pTempDataFilePath, FPATH_MAX);
    FStringConcatenate(".temp", sizeof(".temp"), pTempDataFilePath, FPATH_MAX);

    FFile* pTempDataFile = FFileOpen(pPackage->dataPath, "w");
    if (pTempDataFilePath == NULL)
    {
        FFileClose(&pDataFile);
        return false;
    }

    /* Write the data to the temporary file */
    int32 recordsCount = FArrayCount(pPackage->pRecords);
    for (int32 i = 0; i < recordsCount; i++)
    {
        FPackageRecord* pRecord = (FPackageRecord*)FArrayGet(pPackage->pRecords, i);

        if (pRecord->recordType == E_RECORD_TYPE_REMOVE)
        {
            FFileSeek(pDataFile, 0, E_FILE_SEEK_ORIGIN_SET);

            /* Copy up to the package data */
            char c;
            while((c = FFileReadChar(pDataFile)) != FEOF && FFileTell(pDataFile) < pRecord->dataOffset)
            {
                FFileWriteChar(pTempDataFile, c);
            }

            /* Seek past the data */
            FFileSeek(pDataFile, pRecord->dataCompressedSize, E_FILE_SEEK_ORIGIN_SET);

            /* Write the rest of the file back */
            while((c = FFileReadChar(pDataFile)) != FEOF)
            {
                FFileWriteChar(pTempDataFile, c);
            }
            
            FArrayRemove(pPackage->pRecords, i); i--; /* i-- because we are removing an index, so we dont want to overflow the buffer later */
            FHashMapRemoveWithHashedKey(pPackage->pRecordsMap, pRecord->recordId);

            FPackageRewriteRecords(pPackage);
        }

        FHashMapInsertWithHashedKey(pPackage->pRecordsMap, pRecord->recordId, pRecord);
    }

    FFileClose(&pTempDataFile);
    FFileClose(&pDataFile);

    /* Delete the old data file and rename the temporary file */
    FFileRemove(pPackage->dataPath);
    FFileRename(pTempDataFilePath, pPackage->dataPath);

    return true;
}