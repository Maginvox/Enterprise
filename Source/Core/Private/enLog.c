/* Copyright © 2021 Caden Miller, All Rights Reserved. */

#include <stdio.h>

#include "Core/enFile.h"
#include "Core/enMacro.h"
#include "Core/enString.h"
#include "Core/enLog.h"

static enFile* pLogFile;

bool FLogInit()
{

    char pLogFilename[ENTERPRISE_PATH_MAX_LENGTH] = {0};

    enTime currentTime;    
    enTimeCurrent(&currentTime);
    
    enStringFormatArgument pLogFilenameFormatArguments[] = 
    {
        {enStringFormatType_Int, .value.Integer = currentTime.year},
        {enStringFormatType_Int, .value.Integer = currentTime.month},
        {enStringFormatType_Int, .value.Integer = currentTime.monthDay},
        {enStringFormatType_Int, .value.Integer = currentTime.hour},
        {enStringFormatType_Int, .value.Integer = currentTime.minute},
        {enStringFormatType_Int, .value.Integer = currentTime.second},
    };

    const char* pLogFilenameFormat = "Logs/Log-%i-%i-%i_%i.%i.%i.txt";
    enStringFormat(pLogFilename, ENTERPRISE_NAME_MAX_LENGTH, pLogFilenameFormat, pLogFilenameFormatArguments, COUNT_OF(pLogFilenameFormatArguments));

    pLogFile = enFileOpen(pLogFilename, "w");
    if(pLogFile == NULL)
    {
        return false;
    }

    return true;
}

void FLogShutdown()
{
    if(pLogFile != NULL)
    {
        enFileClose(&pLogFile);
    }
}

/* ====================================================== */
void enLog(const char* pBasis, const char* pContext, const char* pMessage)
{
    if(pBasis == NULL || pContext == NULL || pMessage == NULL)
    {
        return;
    }

    const char pPrintFormat[] = " [ %s ] (%s) | %s\n"; 
    const enStringFormatArgument pFormatArguments[] =
    {
        {enStringFormatType_String, .value.String = (char*)pBasis},
        {enStringFormatType_String, .value.String = (char*)pContext},
        {enStringFormatType_String, .value.String = (char*)pMessage}
    };

    int64 basisLength = enStringLength(pBasis, FSTRING_MAX_LENGTH);
    int64 contextLength = enStringLength(pContext, FSTRING_MAX_LENGTH);
    int64 messageLength = enStringLength(pMessage, FSTRING_MAX_LENGTH);

    if (basisLength + contextLength + messageLength + sizeof(pPrintFormat) >= FSTRING_MAX_LENGTH)
    {
        return;
    }

    char pBuffer[FSTRING_MAX_LENGTH] = {0};
    enStringFormat(pBuffer, sizeof(pBuffer), pPrintFormat, pFormatArguments, COUNT_OF(pFormatArguments));

    printf("%s", pBuffer);

    if(pLogFile != NULL)
    {
        fprintf((FILE*)pLogFile, "%s", pBuffer);
    }

    fflush(stdout);
}

/* ====================================================== */
void enLogInfo(const char* pMessage)
{
    
    /* Get the time and convert it to a string */
    enTime currentTime = {0};
    enTimeCurrent(&currentTime);

    char pTimeString[32] = {0};
    enStringConvertFromTime(&currentTime, pTimeString, 32);

    enLog("INFO", pTimeString, pMessage);
}

/* ====================================================== */
void enLogWarning(const char* pMessage)
{
    /* Get the time and convert it to a string */
    enTime currentTime = {0};
    enTimeCurrent(&currentTime);

    char pTimeString[32] = {0};
    enStringConvertFromTime(&currentTime, pTimeString, 32);

    enLog("WARNING", pTimeString, pMessage);
}

/* ====================================================== */
void enLogError(const char* pMessage)
{
    /* Get the time and convert it to a string */
    enTime currentTime = {0};
    enTimeCurrent(&currentTime);
    
    char pTimeString[32];
    enStringConvertFromTime(&currentTime, pTimeString, 32);

    enLog("ERROR", pTimeString, pMessage);
}

/* ====================================================== */
void enLogDebug(const char* pMessage)
{
    /* Get the time and convert it to a string */
    enTime currentTime = {0};
    enTimeCurrent(&currentTime);

    char pTimeString[32];
    enStringConvertFromTime(&currentTime, pTimeString, 32);

    enLog("DEBUG", pTimeString, pMessage);
}