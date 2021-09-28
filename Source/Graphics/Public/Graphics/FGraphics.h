/* Copyright © 2021 Caden Miller, All Rights Reserved. */

#ifndef __FGRAPHICS_H__
#define __FGRAPHICS_H__

#include "Window/FWindow.h"

typedef struct FGraphicsOptions
{
    FInt32 physicalDevice; /* Cannot change while in runtime */
} FGraphicsOptions;

typedef void* FContext;

bool FGraphicsInitialize(FWindow* pWindow, FContext* pContext, const FGraphicsOptions* pOptions); /* Will automatically register the first window */
void FGraphicsShutdown();

void FGraphicsGetOptions(FGraphicsOptions* pOptions);
void FGraphicsApplyOptions(const FGraphicsOptions* pOptions);

FContext* FGraphicsRegisterWindow(FWindow* pWindow);
void FGraphicsUnRegisterWindow(FWindow* pWindow, FContext* pContext);

#endif