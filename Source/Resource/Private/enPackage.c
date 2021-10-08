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
        .count = enArrayCount(package->records),
    };

    enFileWrite(recordsFile, &header, sizeof(enPackageHeader), 1);
    enFileWrite(recordsFile, package->records, sizeof(enPackageRecord), enArrayCount(package->records));

    return true;
}

enPackage* enPackageOpen(const char* recordPath, const char* packagePath)
{
    if (recordPath == NULL || packagePath == NULL)
    {
        return NULL;
    }

    /* Allocate the package */
    enPackage* package = enCalloc(1, sizeof(enPackage));
    if (!package)
    {
        return NULL;
    }

    enStringCopy(recordPath, FPATH_MAX, package->recordsPath, FPATH_MAX);
    enStringCopy(packagePath, FPATH_MAX, package->dataPath, FPATH_MAX);

    /* Create the records array and hash map */
    package->records = enArrayCreate(sizeof(enPackageRecord), 5);
    if (!package->records)
    {
        enPackageClose(package);
        return NULL;
    }

    /* Create or open the files */
    enFile* recordFile = enFileOpenOrCreate(recordPath, "r");
    enFile* packageFile = enFileOpenOrCreate(packagePath, "r");

    if (!recordFile || !packageFile)
    {
        enPackageClose(package);
        return NULL;
    }

    /* Read the record header */
    enPackageHeader header = {0};
    if (enFileRead(recordFile, &header, sizeof(enPackageHeader), sizeof(enPackageHeader), 1) != 1)
    {
        enFileClose(recordFile);
        enFileClose(packageFile);

        if (!enPackageRewrite(package))
        {
            enFileClose(recordFile);
            enFileClose(packageFile);
            enPackageClose(package);
            enLogError("Could not write default header to package file!");
            return NULL;
        }
    }
    else
    {
        enFileClose(recordFile);
        enFileClose(packageFile);
    }



    /* Check the magic and version */
    if (header.magic != ENTERPRISE_PACKAGE_MAGIC_NUMBER || header.version != ENTERPRISE_PACKAGE_LATEST_VERSION || !enMathIsBetween(header.count, 0, ENTERPRISE_PACKAGE_MAX_RECORDS))
    {

        enPackageClose(package);
        return NULL;
    }



}