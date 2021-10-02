/* Copyright © 2021 Caden Miller, All Rights Reserved. */

#ifndef __FFILE_H__
#define __FFILE_H__

#include "Core/FExport.h"
#include "Core/FTypes.h"

#define FEOF -1
#define FPATH_MAX 256

typedef void* FFile;

typedef enum EFileSeekOrigin {
    E_FILE_SEEK_ORIGIN_BEGIN = 0,
    E_FILE_SEEK_ORIGIN_CURRENT = 1,
    E_FILE_SEEK_ORIGIN_END = 2,
    E_FILE_SEEK_ORIGIN_SET = E_FILE_SEEK_ORIGIN_BEGIN
} EFileSeekOrigin;

FFile*  FFileOpen(const char* pFilename, const char* pMode);
FFile*  FFileOpenOrCreate(const char* pFilename, const char* pMode);
void    FFileClose(FFile** ppFile);
bool    FFileRemove(const char* pFilename);
bool    FFileRename(const char* pOldFilename, const char* pNewFilename);
bool    FFileSeek(FFile* pFile, const int64 offset, const EFileSeekOrigin seekOrigin);
int64   FFileTell(FFile* pFile);
char    FFileReadChar(FFile* pFile);
bool    FFileReadLine(FFile* pFile, char* pBuffer, const int64 bufferMaxLength);
int64   FFileRead(FFile* pFile, void* pBuffer, const int64 bufferMaxLength, const int64 elementSize, const int64 elementCount);
bool    FFileWriteChar(FFile* pFile, const char c);
bool    FFileWrite(FFile* pFile, const void* pBuffer, const int64 bufferLength, const int64 elementSize, const int64 elementCount);

#endif