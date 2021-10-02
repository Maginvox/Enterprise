/* Copyright © 2021 Caden Miller, All Rights Reserved. */

#ifndef __FMEMORY_H__
#define __FMEMORY_H__

#include "Core/FTypes.h"

void* FAllocate(uint64 size);
void* FAllocateZero(uint64 count, uint64 size);
void* FReallocate(void* pMemory, uint64 size);
void FDeallocate(void* pMemory);

void* FMemoryCopy(const void* pSource, void* pDestination, uint64 size);
void* FMemorySet(void* pDestination, uint64 destinationSize, uint8 value);

#endif