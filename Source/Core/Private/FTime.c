#include <time.h>

#include "Core/FTime.h"

void FTimeCurrent(FTime* pTime)
{
    if (pTime == NULL)
    {
        return;
    }

    /* Since the FTime structure is the same as the standard structure */
    time_t rawtime = 0;
    time(&rawtime);

    struct tm* pTimeInfo = localtime(&rawtime);
    
    pTime->second = (FInt8)pTimeInfo->tm_sec;
    pTime->minute = (FInt8)pTimeInfo->tm_min;
    pTime->hour = (FInt8)pTimeInfo->tm_hour;
    pTime->monthDay = (FInt8)pTimeInfo->tm_mday;
    pTime->month = (FInt8)pTimeInfo->tm_mon + 1;
    pTime->year = (FInt32)pTimeInfo->tm_year + 1900;
    pTime->weekDay = (FInt8)pTimeInfo->tm_wday;
    pTime->yearDay = (FInt16)pTimeInfo->tm_yday;
    pTime->isDaylightSavingTime = (FInt8)pTimeInfo->tm_isdst;

}