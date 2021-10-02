/* Copyright © 2021 Caden Miller, All Rights Reserved. */

#ifndef __FIMAGE_H__
#define __FIMAGE_H__

#include "Core/FTypes.h"
#include "Graphics/FGraphics.h"

typedef enum FImageFormat
{
    FImageFormat_R8G8B8A8
} FImageFormat;

typedef enum FImageLayout
{
    FImageLayout_General,
    FImageLayout_Color,
    FImageLayout_DepthStencil,
    FImageLayout_ReadShader, /* Cannot write to */
    FImageLayout_Present,
} FImageLayout;

typedef struct FImageCreateInfo
{
    int32 width;
    int32 hight;
    int32 depth;
    FImageFormat format;
    int32 mipMaps;
    FImageLayout layout;
} FImageCreateInfo;

typedef struct FImage FImage;

FImage FImageCreate(const FImageCreateInfo* pInfo);
void FImageDestroy(FImage image);

#endif