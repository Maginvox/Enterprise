/* Copyright © 2021 Caden Miller, All Rights Reserved. */

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