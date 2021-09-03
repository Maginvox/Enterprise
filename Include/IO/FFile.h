#ifndef __FFILE_H__
#define __FFILE_H__

#include "FTypes.h"

#define FEOF -1
#define FPATH_MAX 256

typedef struct FFile FFile;

typedef enum EFileSeekOrigin {
    E_FILE_SEEK_ORIGIN_BEGIN = 0,
    E_FILE_SEEK_ORIGIN_CURRENT = 1,
    E_FILE_SEEK_ORIGIN_END = 2,
    E_FILE_SEEK_ORIGIN_SET = E_FILE_SEEK_ORIGIN_BEGIN
} EFileSeekOrigin;

FFile* FFileOpen(const char* pFilename, const char* pMode);
FFile* FFileOpenOrCreate(const char* pFilename, const char* pMode);
void FFileClose(FFile** ppFile);
bool FFileRemove(const char* pFilename);
bool FFileRename(const char* pOldFilename, const char* pNewFilename);
bool FFileSeek(FFile* pFile, const FInt64 offset, const EFileSeekOrigin seekOrigin);
FInt64 FFileTell(FFile* pFile);
char FFileReadChar(FFile* pFile);
bool FFileReadLine(FFile* pFile, char* pBuffer, const FInt64 bufferMaxLength);
FInt64 FFileRead(FFile* pFile, void* pBuffer, const FInt64 bufferMaxLength, const FInt64 elementSize, const FInt64 elementCount);
bool FFileWriteChar(FFile* pFile, const char c);
bool FFileWrite(FFile* pFile, const void* pBuffer, const FInt64 bufferLength, const FInt64 elementSize, const FInt64 elementCount);

#endif