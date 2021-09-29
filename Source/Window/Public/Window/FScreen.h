#ifndef __FSCREEN_H__
#define __FSCREEN_H__

#include "Core/FTypes.h"

typedef struct FScreenProperties
{
    FUInt32 width;
    FUInt32 height;
    FUInt32 refreshRate;
} FScreenProperties;

FUInt FScreenGetPrimary(); 
void FScreenGetProperties(FUInt screen, FScreenProperties* pProperties);
bool FScreenEnumerate(FUInt32* pCount, FUInt* pScreens);


#endif