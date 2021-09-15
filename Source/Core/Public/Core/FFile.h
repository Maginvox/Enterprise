#ifndef __FFILE_H__
#define __FFILE_H__

#include "Core/FExport.h"
#include "Core/FTypes.h"

#define FEOF -1
#define FPATH_MAX 256

typedef struct FFile FFile;

typedef enum EFileSeekOrigin {
    E_FILE_SEEK_ORIGIN_BEGIN = 0,
    E_FILE_SEEK_ORIGIN_CURRENT = 1,
    E_FILE_SEEK_ORIGIN_END = 2,
    E_FILE_SEEK_ORIGIN_SET = E_FILE_SEEK_ORIGIN_BEGIN
} EFileSeekOrigin;

FEXPORT FFile* FFileOpen(const char* pFilename, const char* pMode);
FEXPORT FFile* FFileOpenOrCreate(const char* pFilename, const char* pMode);
FEXPORT void FFileClose(FFile** ppFile);
FEXPORT bool FFileRemove(const char* pFilename);
FEXPORT bool FFileRename(const char* pOldFilename, const char* pNewFilename);
FEXPORT bool FFileSeek(FFile* pFile, const FInt64 offset, const EFileSeekOrigin seekOrigin);
FEXPORT FInt64 FFileTell(FFile* pFile);
FEXPORT char FFileReadChar(FFile* pFile);
FEXPORT bool FFileReadLine(FFile* pFile, char* pBuffer, const FInt64 bufferMaxLength);
FEXPORT FInt64 FFileRead(FFile* pFile, void* pBuffer, const FInt64 bufferMaxLength, const FInt64 elementSize, const FInt64 elementCount);
FEXPORT bool FFileWriteChar(FFile* pFile, const char c);
FEXPORT bool FFileWrite(FFile* pFile, const void* pBuffer, const FInt64 bufferLength, const FInt64 elementSize, const FInt64 elementCount);

#endif