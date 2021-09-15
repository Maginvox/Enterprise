#ifndef __FTIME_H__
#define __FTIME_H__

#include "FExport.h"
#include "FTypes.h"

typedef struct FTime
{
    FInt32 second;
    FInt32 minute;
    FInt32 hour;
    FInt32 monthDay;
    FInt32 month;
    FInt32 year;
    FInt32 weekDay;
    FInt32 yearDay;
    FInt32 isDaylightSavingTime;
} FTime;

void FTimeCurrent(FTime* time);

#endif