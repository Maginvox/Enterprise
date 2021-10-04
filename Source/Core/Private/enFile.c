/* Copyright © 2021 Caden Miller, All Rights Reserved. */

#include <stdio.h>

#include "Core/enFile.h"
#include "Core/enLog.h"

/* ====================================================== */
enFile* enFileOpen(const char* pFilename, const char* pMode)
{
    if (pFilename == NULL || pMode == NULL)
    {
        return NULL;
    }

    enFile* pFile = NULL;
    #ifdef ENTERPRISE_WINDOWS
        if (!fopen_s((FILE**)&pFile, pFilename, pMode))
        {
            return NULL;
        }
    #else
        pFile = (enFile*)fopen(pFilename, pMode);
    #endif

    return pFile;
}

/* ====================================================== */
enFile* enFileOpenOrCreate(const char* pFilename, const char* pMode)
{
    enFile* pFile = enFileOpen(pFilename, "r");

    if (pFile == NULL)
    {
        pFile = enFileOpen(pFilename, "w");
        if (pFile == NULL)
        {
            return NULL;
        }
    }

    enFileClose(&pFile);
    return enFileOpen(pFilename, pMode);
}

/* ====================================================== */
void enFileClose(enFile** ppFile)
{
    if (ppFile == NULL || *ppFile == NULL)
    {
        return;
    }

    enFile* pFile = *ppFile;

    fclose((FILE*)pFile);
    *ppFile = NULL;
}

/* ====================================================== */
bool enFileRemove(const char* pFilename)
{
    if (pFilename == NULL)
    {
        return false;
    }

    return remove(pFilename) == 0;
}

/* ====================================================== */
bool enFileRename(const char* pOldFilename, const char* pNewFilename)
{
    if (pOldFilename == NULL || pNewFilename == NULL)
    {
        return false;
    }

    return rename(pOldFilename, pNewFilename) == 0;
}

/* ====================================================== */
bool enFileSeek(enFile* pFile, const int64 offset, const enSeek seekOrigin)
{
    if (pFile == NULL)
    {
        return false;
    }

    int seekOriginValue = SEEK_SET;
    switch (seekOrigin)
    {
    case enFileSeekOrigin_Begin:
        seekOriginValue = SEEK_SET;
        break;
    case enFileSeek_Current:
        seekOriginValue = SEEK_CUR;
        break;
    case enFileSeek_End:
        seekOriginValue = SEEK_END;
        break;
    default:
        return false;
    }

    return fseek((FILE*)pFile, (long)offset, seekOriginValue) == 0;
}

/* ====================================================== */
int64 enFileTell(enFile* pFile)
{
    if (pFile == NULL)
    {
        return false;
    }

    return ftell((FILE*)pFile);
}

/* ====================================================== */
char FFileReadChar(enFile* pFile)
{
    if (pFile == NULL)
    {
        return false;
    }

    return (char)fgetc((FILE*)pFile);
}

/* ====================================================== */
bool enFileReadLine(enFile* pFile, char* pBuffer, const int64 bufferMaxLength)
{
    if (pFile == NULL || pBuffer == NULL || bufferMaxLength == 0)
    {
        return false;
    }

    return fgets(pBuffer, (int)bufferMaxLength, (FILE*)pFile) != NULL;
}

/* ====================================================== */
int64 enFileRead(enFile* pFile, void* pBuffer, const int64 bufferMaxLength, const int64 elementSize, const int64 elementCount)
{
    if (pFile == NULL || pBuffer == NULL || bufferMaxLength <= 0 || elementSize <= 0 || elementCount <= 0)
    {
        return 0;
    }

    if (elementSize * elementCount > bufferMaxLength)
    {
        return 0;
    }

    return fread(pBuffer, elementSize, elementCount, (FILE*)pFile);
}

/* ====================================================== */
bool enFileWriteChar(enFile* pFile, const char c)
{
    if (pFile == NULL)
    {
        return false;
    }

    return fputc(c, (FILE*)pFile) == c;
}

/* ====================================================== */
bool enFileWrite(enFile* pFile, const void* pBuffer, const int64 bufferLength, const int64 elementSize, const int64 elementCount)
{
    if (pFile == NULL || pBuffer == NULL || bufferLength <= 0 || elementSize <= 0 || elementCount <= 0)
    {
        return false;
    }

    if (elementSize * elementCount > bufferLength)
    {
        return false;
    }

    return (int64)fwrite(pBuffer, elementSize, elementCount, (FILE*)pFile) == elementCount;
}