/*
	 Notice: Copyright © 2021 Caden Miller, All Rights Reserved.
 */

#ifndef __FPLATFORM_H__
#define __FPLATFORM_H__

#include "FTypes.h"

/* FPlatformAPI
	Platforms that only support one API eg. Windows, MacOS, Consoles will not be listed. */
typedef enum FPlatformAPI
{
	FPlatformAPI_X11,
	FPlatformAPI_Wayland,
} FPlatformAPI;

typedef struct FPlatformCreateInfo
{
	FPlatformAPI api;
	FInt32 desiredWidth;
	FInt32 desiredHeight;
} FPlatformCreateInfo;

typedef struct FPlatform FPlatform;

FPlatform* FPlatformCreate(const FPlatformCreateInfo* pInfo);
void FPlatformDestroy(FPlatform** ppPlatform);

#endif