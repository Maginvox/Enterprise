/* Copyright © 2021 Caden Miller, All Rights Reserved. */

#include <stdio.h>

#include "Core/FFile.h"
#include "Core/FMacro.h"
#include "Core/FString.h"
#include "Core/FLog.h"

static FFile* pLogFile;

bool FLogInit()
{

    char pLogFilename[ENTERPRISE_PATH_MAX_LENGTH] = {0};

    FTime currentTime;    
    FTimeCurrent(&currentTime);
    
    FStringFormatArgument pLogFilenameFormatArguments[] = 
    {
        {E_STRING_FORMAT_TYPE_INT, .value.Integer = currentTime.year},
        {E_STRING_FORMAT_TYPE_INT, .value.Integer = currentTime.month},
        {E_STRING_FORMAT_TYPE_INT, .value.Integer = currentTime.monthDay},
        {E_STRING_FORMAT_TYPE_INT, .value.Integer = currentTime.hour},
        {E_STRING_FORMAT_TYPE_INT, .value.Integer = currentTime.minute},
        {E_STRING_FORMAT_TYPE_INT, .value.Integer = currentTime.second},
    };

    const char* pLogFilenameFormat = "Logs/Log-%i-%i-%i_%i.%i.%i.txt";
    FStringFormat(pLogFilename, ENTERPRISE_NAME_MAX_LENGTH, pLogFilenameFormat, pLogFilenameFormatArguments, FCOUNT_OF(pLogFilenameFormatArguments));

    pLogFile = FFileOpen(pLogFilename, "w");
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
        FFileClose(&pLogFile);
    }
}

/* ====================================================== */
void FLog(const char* pBasis, const char* pContext, const char* pMessage)
{
    if(pBasis == NULL || pContext == NULL || pMessage == NULL)
    {
        return;
    }

    const char pPrintFormat[] = " [ %s ] (%s) | %s\n"; 
    const FStringFormatArgument pFormatArguments[] =
    {
        {E_STRING_FORMAT_TYPE_STRING, .value.String = (char*)pBasis},
        {E_STRING_FORMAT_TYPE_STRING, .value.String = (char*)pContext},
        {E_STRING_FORMAT_TYPE_STRING, .value.String = (char*)pMessage}
    };

    FInt64 basisLength = FStringLength(pBasis, FSTRING_MAX_LENGTH);
    FInt64 contextLength = FStringLength(pContext, FSTRING_MAX_LENGTH);
    FInt64 messageLength = FStringLength(pMessage, FSTRING_MAX_LENGTH);

    if (basisLength + contextLength + messageLength + sizeof(pPrintFormat) >= FSTRING_MAX_LENGTH)
    {
        return;
    }

    char pBuffer[FSTRING_MAX_LENGTH] = {0};
    FStringFormat(pBuffer, sizeof(pBuffer), pPrintFormat, pFormatArguments, FCOUNT_OF(pFormatArguments));

    printf("%s", pBuffer);

    if(pLogFile != NULL)
    {
        fprintf((FILE*)pLogFile, "%s", pBuffer);
    }
}

/* ====================================================== */
void FLogInfo(const char* pMessage)
{
    
    /* Get the time and convert it to a string */
    FTime currentTime = {0};
    FTimeCurrent(&currentTime);

    char pTimeString[32] = {0};
    FStringConvertFromTime(&currentTime, pTimeString, 32);

    FLog("INFO", pTimeString, pMessage);
}

/* ====================================================== */
void FLogWarning(const char* pMessage)
{
    /* Get the time and convert it to a string */
    FTime currentTime = {0};
    FTimeCurrent(&currentTime);

    char pTimeString[32] = {0};
    FStringConvertFromTime(&currentTime, pTimeString, 32);

    FLog("WARNING", pTimeString, pMessage);
}

/* ====================================================== */
void FLogError(const char* pMessage)
{
    /* Get the time and convert it to a string */
    FTime currentTime = {0};
    FTimeCurrent(&currentTime);
    
    char pTimeString[32];
    FStringConvertFromTime(&currentTime, pTimeString, 32);

    FLog("ERROR", pTimeString, pMessage);
}

/* ====================================================== */
void FLogDebug(const char* pMessage)
{
    /* Get the time and convert it to a string */
    FTime currentTime = {0};
    FTimeCurrent(&currentTime);

    char pTimeString[32];
    FStringConvertFromTime(&currentTime, pTimeString, 32);

    FLog("DEBUG", pTimeString, pMessage);
}