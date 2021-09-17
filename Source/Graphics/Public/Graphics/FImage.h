/*
   Notice: Copyright © 2021 Caden Miller, All Rights Reserved.
 */

#ifndef __FIMAGE_H__
#define __FIMAGE_H__

#include "Core/FExport.h"
#include "Core/FTypes.h"
#include "Graphics/FGraphicsContext.h"

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
    FInt32 width;
    FInt32 hight;
    FInt32 depth;
    FImageFormat format;
    FInt32 mipMaps;
    FImageLayout layout;
} FImageCreateInfo;

typedef struct FImage FImage;

typedef FImage* (*FImageCreateFunction)(FGraphicsContext* pContext, const FImageCreateInfo* pInfo);
typedef void (*FImageDestroyFunction)(FGraphicsContext* pContext, FImage** ppImage);

FImage* FImageCreate(FGraphicsContext* pContext, const FImageCreateInfo* pInfo);
void FImageDestroy(FGraphicsContext* pContext, FImage** ppImage);

#endif