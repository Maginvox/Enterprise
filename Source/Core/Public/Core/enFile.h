/* Copyright © 2021 Caden Miller, All Rights Reserved. */

#ifndef __FFILE_H__
#define __FFILE_H__

#include "Core/enTypes.h"

#define FEOF -1
#define FPATH_MAX 256

typedef enum enSeek {
    enFileSeekOrigin_Begin = 0,
    enFileSeek_Current = 1,
    enFileSeek_End = 2,
    enFileSeek_Set = enFileSeekOrigin_Begin
} enSeek;

typedef void* enFile;

enFile* enFileOpen(const char* pFilename, const char* pMode);
enFile* enFileOpenOrCreate(const char* pFilename, const char* pMode);
void    enFileClose(enFile* pFile);
bool    enFileRemove(const char* pFilename);
bool    enFileRename(const char* pOldFilename, const char* pNewFilename);
bool    enFileSeek(enFile* pFile, const int64 offset, const enSeek origin);
int64   enFileTell(enFile* pFile);
char    enFileReadChar(enFile* pFile);
bool    enFileReadLine(enFile* pFile, char* pBuffer, const int64 bufferMaxLength);
int64   enFileRead(enFile* pFile, void* pBuffer, const int64 elementSize, const int64 elementCount);
bool    enFileWriteChar(enFile* pFile, const char c);
int64   enFileWrite(enFile* pFile, const void* pBuffer, const int64 elementSize, const int64 elementCount);

#endif