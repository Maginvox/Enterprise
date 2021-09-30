/* Copyright © 2021 Caden Miller, All Rights Reserved. */

#ifndef __FGRAPHICS_H__
#define __FGRAPHICS_H__

#include "FWindow.h"

typedef struct FGraphicsOptions
{
    FInt32 physicalDevice; /* Cannot change while in runtime */
} FGraphicsOptions;

typedef void* FContext;

bool FGraphicsInitialize(const FWindowInfo* pWindowInfo,  const FGraphicsOptions* pOptions); /* Will automatically register the first window */
void FGraphicsShutdown();

void FGraphicsGetOptions(FGraphicsOptions* pOptions);
void FGraphicsApplyOptions(const FGraphicsOptions* pOptions);

void FGraphicsNextFrame();

#endif