/* Copyright © 2021 Caden Miller, All Rights Reserved. */

#ifndef __FTIME_H__
#define __FTIME_H__

#include "FTypes.h"

typedef struct FTime
{
    int32 second;
    int32 minute;
    int32 hour;
    int32 monthDay;
    int32 month;
    int32 year;
    int32 weekDay;
    int32 yearDay;
    int32 isDaylightSavingTime;
} FTime;

void FTimeCurrent(FTime* time);

#endif