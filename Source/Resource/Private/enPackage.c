/* Copyright © 2021 Caden Miller, All Rights Reserved. */

#include "Core/enMemory.h"
#include "Core/enHash.h"
#include "Core/enLog.h"
#include "Core/enFile.h"

#include "Resource/enPackage.h"

bool FPackageRewriteRecords(const FPackage* pPackage)
{
    if (pPackage == NULL)
    {
        return false;
    }

    enFile* pRecordsFile = enFileOpen(pPackage->recordsPath, "w");
    if (pRecordsFile == NULL)
    {
        return false;
    }

    /* Write the header */
    const FPackageHeader header = {
        ENTERPRISE_PACKAGE_MAGIC_NUMBER,
        ENTERPRISE_PACKAGE_LATEST_VERSION,
        pPackage->pRecords->count
    };

    if (enFileWrite(pRecordsFile, &header, sizeof(FPackageHeader), sizeof(FPackageHeader), 1) != 1)
    {
        enFileClose(&pRecordsFile);
        return false;
    }

    /* Write all of the records */
    FPackageRecord* pRecords = enArrayData(pPackage->pRecords);
    int32 recordsCount = enArrayCount(pPackage->pRecords);

    if (pRecords != NULL && recordsCount != 0)
    {
        if (enFileWrite(pRecordsFile, pRecords, sizeof(FPackageRecord) * recordsCount, sizeof(FPackageRecord), recordsCount) != recordsCount)
        {
            enFileClose(&pRecordsFile);
            return false;
        }
    }

    /* Write the footer */
    const FPackageFooter footer = {
        ENTERPRISE_PACKAGE_MAGIC_NUMBER,
        ENTERPRISE_PACKAGE_LATEST_VERSION
    };

    if (enFileWrite(pRecordsFile, &footer, sizeof(FPackageFooter), sizeof(FPackageFooter), 1) != 1)
    {
        enFileClose(&pRecordsFile);
        return false;
    }

    enFileClose(&pRecordsFile);
    return true;
}

bool FPackageRemapRecords(FPackage* pPackage)
{

    if (pPackage == NULL)
    {
        return false;
    }

    int32 recordsCount = enArrayCount(pPackage->pRecords);
    for (int32 i = 0; i < recordsCount; i++)
    {
        FPackageRecord* pRecord = enArrayGet(pPackage->pRecords, i);
        if (!enHashMapInsertWithHashedKey(pPackage->pRecordsMap, pRecord->recordId, pRecord))
        {
            return false;
        }
    }

    return true;
}

/* ====================================================== */
FPackage* enPackageCreate(const char* pRecordsPath, const char* pDataPath)
{
    if (pRecordsPath == NULL || pDataPath == NULL)
    {
        return NULL;
    }


    FPackage* pPackage = enCalloc(1, sizeof(FPackage));
    if (pPackage == NULL)
    {
        return NULL;
    }

    /* Open the records file */
    enFile* pRecordsFile = enFileOpenOrCreate(pRecordsPath, "r+");
    if (pRecordsFile == NULL)
    {
        return NULL;
    }

    /* Read for the header */
    FPackageHeader header = {0};
    if (enFileRead(pRecordsFile, &header, sizeof(FPackageHeader), sizeof(FPackageHeader), 1) != 1)
    {
        /* Generate a default package */
        if (!FPackageRewriteRecords(pPackage))
        {
            enPackageDestroy(&pPackage);
            return NULL;
        }
    }

    return pPackage;
}

/* ====================================================== */
void enPackageDestroy(FPackage** ppPackage)
{
    if (ppPackage == NULL || *ppPackage == NULL)
    {
        return;
    }

    FPackage* pPackage = *ppPackage;

    if (pPackage->pRecords != NULL)
    {
        enArrayDestroy(&pPackage->pRecords);
    }

    if (pPackage->pRecordsMap != NULL)
    {
        enHashMapDestroy(&pPackage->pRecordsMap);
    }

    enFree(pPackage);
}

/* ====================================================== */
bool enPackageRecordAppend(FPackage* pPackage, const FPackageRecord* pRecord)
{
    if (pPackage == NULL || pRecord == NULL)
    {
        return false;
    }

    /* Add the record to the array */
    enArrayAdd(pPackage->pRecords, pRecord);
    enHashMapInsertWithHashedKey(pPackage->pRecordsMap, pRecord->recordId, (void*)pRecord);


    /* Rewrite the records */
    return FPackageRewriteRecords(pPackage);
}

const FPackageRecord* enPackageGetRecord(FPackage* pPackage, const char* pName)
{
    if (pPackage == NULL || pName == NULL)
    {
        return NULL;
    }

    return enHashMapGet(pPackage->pRecordsMap, pName);
}

/* ====================================================== */
bool enPackageRecordMarkRemove(FPackage* pPackage, const char* pNameId)
{
    if (pPackage == NULL || pNameId == NULL)
    {
        return false;
    }

    FPackageRecord* pRecord = enHashMapGet(pPackage->pRecordsMap, pNameId);
    if (pRecord == NULL)
    {
        return false;
    }

    pRecord->recordType = E_RECORD_TYPE_REMOVE;

    /* Open the records file and write the record */
    return enPackageRecordChange(pPackage, pRecord);
}

/* ====================================================== */
bool enPackageRecordChange(FPackage* pPackage, const FPackageRecord* pNewRecord)
{
    if (pPackage == NULL || pNewRecord == NULL)
    {
        return false;
    }

    /* Copy the new record to the old record */
    FPackageRecord* pRecord = (FPackageRecord*)enHashMapGetWithHashedKey(pPackage->pRecordsMap, pNewRecord->recordId);
    if (pRecord == NULL)
    {
        return false;
    }

    enMemoryCopy(pNewRecord, pRecord, sizeof(FPackageRecord));
 
    /* Rewrite the records */
    return FPackageRewriteRecords(pPackage);
}

/* ====================================================== */
bool enPackageDataAppend(FPackage* pPackage, const void* pData, int64 size, int64* pOffset)
{
    if (pPackage == NULL || pData == NULL || size < 1 || pOffset == NULL)
    {
        return false;
    }

    enFile* pDataFile = enFileOpen(pPackage->dataPath, "a");
    if (pDataFile == NULL)
    {
        return false;
    }

    *pOffset = enFileTell(pDataFile);

    if (enFileWrite(pDataFile, pData, size, size, 1) != 1)
    {
        enFileClose(&pDataFile);
        return false;
    }

    enFileClose(&pDataFile);
    return true;
}

/* ====================================================== */
bool enPackageDefragment(FPackage* pPackage)
{
    if (pPackage == NULL)
    {
        return false;
    }

    enFile* pDataFile = enFileOpen(pPackage->dataPath, "r+");
    if (pDataFile == NULL)
    {
        return false;
    }

    /* Open a temporary data file */
    char pTempDataFilePath[FPATH_MAX] = {0};
    enStringCopy(pPackage->dataPath, FPATH_MAX, pTempDataFilePath, FPATH_MAX);
    enStringConcatenate(".temp", sizeof(".temp"), pTempDataFilePath, FPATH_MAX);

    enFile* pTempDataFile = enFileOpen(pPackage->dataPath, "w");
    if (pTempDataFilePath == NULL)
    {
        enFileClose(&pDataFile);
        return false;
    }

    /* Write the data to the temporary file */
    int32 recordsCount = enArrayCount(pPackage->pRecords);
    for (int32 i = 0; i < recordsCount; i++)
    {
        FPackageRecord* pRecord = (FPackageRecord*)enArrayGet(pPackage->pRecords, i);

        if (pRecord->recordType == E_RECORD_TYPE_REMOVE)
        {
            enFileSeek(pDataFile, 0, enFileSeek_Set);

            /* Copy up to the package data */
            char c;
            while((c = enFileReadChar(pDataFile)) != FEOF && enFileTell(pDataFile) < pRecord->dataOffset)
            {
                enFileWriteChar(pTempDataFile, c);
            }

            /* Seek past the data */
            enFileSeek(pDataFile, pRecord->dataCompressedSize, enFileSeek_Set);

            /* Write the rest of the file back */
            while((c = enFileReadChar(pDataFile)) != FEOF)
            {
                enFileWriteChar(pTempDataFile, c);
            }
            
            enArrayRemove(pPackage->pRecords, i); 
            i--; /* i-- because we are removing an index, so we dont want to overflow the buffer later */
            enHashMapRemoveWithHashedKey(pPackage->pRecordsMap, pRecord->recordId);

            FPackageRewriteRecords(pPackage);
        }

        enHashMapInsertWithHashedKey(pPackage->pRecordsMap, pRecord->recordId, pRecord);
    }

    enFileClose(&pTempDataFile);
    enFileClose(&pDataFile);

    /* Delete the old data file and rename the temporary file */
    enFileRemove(pPackage->dataPath);
    enFileRename(pTempDataFilePath, pPackage->dataPath);

    return true;
}