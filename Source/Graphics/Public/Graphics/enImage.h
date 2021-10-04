/* Copyright © 2021 Caden Miller, All Rights Reserved. */

#ifndef __FIMAGE_H__
#define __FIMAGE_H__

#include "Core/FTypes.h"
#include "Graphics/enGraphics.h"

typedef enum enImageFormat
{
    enImageFormat_R8G8B8A8
} enImageFormat;

typedef struct
{
    int32 width;
    int32 hight;
    enImageFormat format;
    int32 mipMaps;
} enImageCreateInfo;

typedef void* enImage;

enImage enImageCreate(const enImageCreateInfo* pInfo);
void enImageDestroy(enImage image);

#endif