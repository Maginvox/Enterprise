#ifndef __FMEMORY_H__
#define __FMEMORY_H__

#include "Core/FTypes.h"

void* FAllocate(FUInt64 size);
void* FAllocateZero(FUInt64 count, FUInt64 size);
void* FReallocate(void* pMemory, FUInt64 size);
void FDeallocate(void* pMemory);

void* FMemoryCopy(const void* pSource, void* pDestination, FInt64 size);
void* FMemorySet(void* pDestination, FInt64 destinationSize, FInt value);

#endif