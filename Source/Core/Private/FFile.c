/* Copyright © 2021 Caden Miller, All Rights Reserved. */

#include <stdio.h>

#include "Core/FFile.h"
#include "Core/FLog.h"

/* ====================================================== */
FFile* FFileOpen(const char* pFilename, const char* pMode)
{
    if (pFilename == NULL || pMode == NULL)
    {
        return NULL;
    }

    FFile* pFile = NULL;
    #ifdef ENTERPRISE_WINDOWS
        if (!fopen_s((FILE**)&pFile, pFilename, pMode))
        {
            return NULL;
        }
    #else
        pFile = (FFile*)fopen(pFilename, pMode);
    #endif

    return pFile;
}

/* ====================================================== */
FFile* FFileOpenOrCreate(const char* pFilename, const char* pMode)
{
    FFile* pFile = FFileOpen(pFilename, "r");

    if (pFile == NULL)
    {
        pFile = FFileOpen(pFilename, "w");
        if (pFile == NULL)
        {
            return NULL;
        }
    }

    FFileClose(&pFile);
    return FFileOpen(pFilename, pMode);
}

/* ====================================================== */
void FFileClose(FFile** ppFile)
{
    if (ppFile == NULL || *ppFile == NULL)
    {
        return;
    }

    FFile* pFile = *ppFile;

    fclose((FILE*)pFile);
    *ppFile = NULL;
}

/* ====================================================== */
bool FFileRemove(const char* pFilename)
{
    if (pFilename == NULL)
    {
        return false;
    }

    return remove(pFilename) == 0;
}

/* ====================================================== */
bool FFileRename(const char* pOldFilename, const char* pNewFilename)
{
    if (pOldFilename == NULL || pNewFilename == NULL)
    {
        return false;
    }

    return rename(pOldFilename, pNewFilename) == 0;
}

/* ====================================================== */
bool FFileSeek(FFile* pFile, const int64 offset, const EFileSeekOrigin seekOrigin)
{
    if (pFile == NULL)
    {
        return false;
    }

    int seekOriginValue = SEEK_SET;
    switch (seekOrigin)
    {
    case E_FILE_SEEK_ORIGIN_BEGIN:
        seekOriginValue = SEEK_SET;
        break;
    case E_FILE_SEEK_ORIGIN_CURRENT:
        seekOriginValue = SEEK_CUR;
        break;
    case E_FILE_SEEK_ORIGIN_END:
        seekOriginValue = SEEK_END;
        break;
    default:
        return false;
    }

    return fseek((FILE*)pFile, (long)offset, seekOriginValue) == 0;
}

/* ====================================================== */
int64 FFileTell(FFile* pFile)
{
    if (pFile == NULL)
    {
        return false;
    }

    return ftell((FILE*)pFile);
}

/* ====================================================== */
char FFileReadChar(FFile* pFile)
{
    if (pFile == NULL)
    {
        return false;
    }

    return (char)fgetc((FILE*)pFile);
}

/* ====================================================== */
bool FFileReadLine(FFile* pFile, char* pBuffer, const int64 bufferMaxLength)
{
    if (pFile == NULL || pBuffer == NULL || bufferMaxLength == 0)
    {
        return false;
    }

    return fgets(pBuffer, (int)bufferMaxLength, (FILE*)pFile) != NULL;
}

/* ====================================================== */
int64 FFileRead(FFile* pFile, void* pBuffer, const int64 bufferMaxLength, const int64 elementSize, const int64 elementCount)
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
bool FFileWriteChar(FFile* pFile, const char c)
{
    if (pFile == NULL)
    {
        return false;
    }

    return fputc(c, (FILE*)pFile) == c;
}

/* ====================================================== */
bool FFileWrite(FFile* pFile, const void* pBuffer, const int64 bufferLength, const int64 elementSize, const int64 elementCount)
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