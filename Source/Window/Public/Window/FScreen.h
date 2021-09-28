#ifndef __FSCREEN_H__
#define __FSCREEN_H__

#include "Core/FTypes.h"

typedef FUInt32 FScreen;

typedef struct FScreenProperties
{
    FUInt32 width;
    FUInt32 height;
    FUInt32 refreshRate;
} FScreenProperties;

FScreen FScreenGetPrimary(); 
void FScreenGetProperties(FScreenProperties* pProperties);
bool FScreenEnumerate(FUInt32* pCount, FScreen* pScreens);


#endif