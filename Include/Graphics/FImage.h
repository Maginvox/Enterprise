/*
   Notice: Copyright © 2021 Caden Miller, All Rights Reserved.
 */

#ifndef __FIMAGE_H__
#define __FIMAGE_H__

#include "FTypes.h"
#include "FRenderContext.h"

typedef enum FImageFormat
{
    FImageFormat_SRGB8
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
    FInt32 width;
    FInt32 hight;
    FInt32 depth;
    FImageFormat format;
    FInt32 mipMaps;
    FImageLayout layout;
} FImageCreateInfo;

typedef struct FImage FImage;

typedef FImage* (*PFN_FImageCreate)(FRenderContext* pAPI, const FImageCreateInfo* pInfo);
typedef FImage* (*PFN_FImageDestroy)(FRenderContext* pAPI, FImage* pImage);

FImage* FImageCreate(FRenderContext* pAPI, const FImageCreateInfo* pInfo);
void FImageDestroy(FRenderContext* pAPI, FImage** ppImage);

#endif