#ifndef __FMEMORY_H__
#define __FMEMORY_H__

#include "Core/FExport.h"
#include "Core/FTypes.h"

FEXPORT void* FAllocate(FUInt64 size);
FEXPORT void* FAllocateZero(FUInt64 count, FUInt64 size);
FEXPORT void* FReallocate(void* pMemory, FUInt64 size);
FEXPORT void FDeallocate(void* pMemory);

FEXPORT void* FMemoryCopy(const void* pSource, void* pDestination, FInt64 size);
FEXPORT void* FMemorySet(void* pDestination, FInt64 destinationSize, FInt value);

#endif