/* Copyright © 2021 Caden Miller, All Rights Reserved. */

#include <time.h>

#include "Core/FTime.h"

void enTimeCurrent(enTime* pTime)
{
    if (pTime == NULL)
    {
        return;
    }

    /* Since the enTime structure is the same as the standard structure */
    time_t rawtime = 0;
    time(&rawtime);

    struct tm* pTimeInfo = NULL;
    #ifdef ENTERPRISE_WINDOWS
        struct tm timeInfo = {0};
        if (!localtime_s(&timeInfo, &rawtime))
        {
            *(pTime) = (enTime){0};
            return;
        }
        pTimeInfo = &timeInfo;
    #else
        pTimeInfo = localtime(&rawtime);
    #endif

    pTime->second = (int8)pTimeInfo->tm_sec;
    pTime->minute = (int8)pTimeInfo->tm_min;
    pTime->hour = (int8)pTimeInfo->tm_hour;
    pTime->monthDay = (int8)pTimeInfo->tm_mday;
    pTime->month = (int8)pTimeInfo->tm_mon + 1;
    pTime->year = (int32)pTimeInfo->tm_year + 1900;
    pTime->weekDay = (int8)pTimeInfo->tm_wday;
    pTime->yearDay = (int16)pTimeInfo->tm_yday;
    pTime->isDaylightSavingTime = (int8)pTimeInfo->tm_isdst;

}