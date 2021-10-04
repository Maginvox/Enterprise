/* Copyright © 2021 Caden Miller, All Rights Reserved. */

#ifndef __ENTERPRISE_MEMORY_H__
#define __ENTERPRISE_MEMORY_H__

#include "Core/enTypes.h"

void* enAllocate(uint64 size);
void* enAllocateZero(uint64 count, uint64 size);
void* enReallocate(void* pMemory, uint64 size);
void enDeallocate(void* pMemory);

void* enMemoryCopy(const void* pSource, void* pDestination, uint64 size);
void* enMemorySet(void* pDestination, uint64 destinationSize, uint8 value);

#endif