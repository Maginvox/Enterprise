/* Copyright © 2021 Caden Miller, All Rights Reserved. */

#ifndef __ENTERPRISE_MEMORY_H__
#define __ENTERPRISE_MEMORY_H__

#include "Core/enTypes.h"

void* enMalloc(uint64 size);
void* enCalloc(uint64 count, uint64 size);
void* enRealloc(void* pMemory, uint64 size);
void enFree(void* pMemory);

void* enMemoryCopy(void* pDestination, const void* pSource, uint64 size);
void* enMemorySet(void* pDestination, uint64 destinationSize, uint8 value);

#endif