/* Copyright © 2021 Caden Miller, All Rights Reserved. */

#include "Core/enMemory.h"
#include "Core/enHash.h"
#include "Core/enLog.h"
#include "Core/enFile.h"

#include "Resource/enPackage.h"

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

    /* Create or open the files */
    enFile* recordFile = enFileOpenOrCreate(recordPath, "r");
    enFile* packageFile = enFileOpenOrCreate(packagePath, "r");

    if (!recordFile || !packageFile)
    {
        enPackageClose(package);
        return NULL;
    }

    /* Read the manifest */

        /* If not, rewrite the default manifest */

    /* Create the records array and hash map */
    package->records = enCalloc(5, sizeof(enPackageRecord));
    package->recordsMap = enHashMapCreate(ENTERPRISE_PACKAGE_MAX_RECORDS);
    if (!package->records || !package->recordsMap)
    {
        enPackageClose(package);
        return NULL;
    }

}