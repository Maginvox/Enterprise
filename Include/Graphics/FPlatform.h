/*
	 Notice: Copyright © 2021 Caden Miller, All Rights Reserved.
 */

#ifndef __FPLATFORM_H__
#define __FPLATFORM_H__

typedef struct FPlatformCreateInfo
{
	FInt32 desiredWidth;
	FInt32 desiredHeight;
} FPlatformCreateInfo;

typedef struct FPlatform 
{
	
} FPlatform;

FPlatform* FPlatformCreate();

#endif