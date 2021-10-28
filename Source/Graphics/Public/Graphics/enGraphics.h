/* Copyright © 2021 Caden Miller, All Rights Reserved. */

#ifndef __FGRAPHICS_H__
#define __FGRAPHICS_H__

#include "enWindow.h"

typedef struct
{
    uint32 physicalDeviceVulkan;
} enGraphicsOptions;

bool enGraphicsInitialize(const enWindowInfo* pWindowInfo,  const enGraphicsOptions* pOptions); /* Will automatically register the first window */
void enGraphicsShutdown();

void enGraphicsGetOptions(enGraphicsOptions* pOptions);
void enGraphicsApplyOptions(const enGraphicsOptions* pOptions);

void enGraphicsNextFrame();

#endif