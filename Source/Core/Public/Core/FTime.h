/* Copyright © 2021 Caden Miller, All Rights Reserved. */

#ifndef __ENTERPRISE_TIME_H__
#define __ENTERPRISE_TIME_H__

#include "enTypes.h"

typedef struct enTime
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
} enTime;

void enTimeCurrent(enTime* time);

#endif