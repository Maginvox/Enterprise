/* Copyright © 2021 Caden Miller, All Rights Reserved. */

#ifndef __FGRAPHICS_H__
#define __FGRAPHICS_H__

#include "FWindow.h"

typedef struct
{
    int32 physicalDevice; /* Cannot change while in runtime */
} enGraphicsOptions;

bool enGraphicsInitialize(const FWindowInfo* pWindowInfo,  const FGraphicsOptions* pOptions); /* Will automatically register the first window */
void enGraphicsShutdown();

void enGraphicsGetOptions(FGraphicsOptions* pOptions);
void enGraphicsApplyOptions(const FGraphicsOptions* pOptions);

void enGraphicsNextFrame();

#endif