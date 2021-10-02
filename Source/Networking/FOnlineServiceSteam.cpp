/* Copyright © 2021 Caden Miller, All Rights Reserved. */

#include <steam/steam_api_flat.h>

extern "C" {

#include "Core/FMacro.h"
#include "Core/FMemory.h"
#include "Core/FString.h"
#include "Core/FThread.h"
#include "Networking/FOnlineService.h"


typedef struct FOnlineServiceSteam
{
    ISteamUser* pSteamUser;
    ISteamUtils* pSteamUtils;
    ISteamFriends* pSteamFriends;
    ISteamMatchmaking* pSteamMatchmaking;

    uint64_steamid userId;
    uint64_steamid lobbyId;
    uint64_steamid gameId;

    FMutex* pLobbyCreatedResponseMutex;
    EOnlineServiceResponse lobbyCreatedResponse;
    void OnLobbyCreated(LobbyCreated_t *pCallback, bool bIOFailure);
	CCallResult<FOnlineServiceSteam, LobbyCreated_t> m_SteamCallResultLobbyCreated;
    

} FOnlineServiceSteam;

/* ====================================================== */ 
void FOnlineServiceSteam::OnLobbyCreated(LobbyCreated_t* pLobby, bool bIOFailure)
{

    /* Lock the lobby created response variable */
    FMutexLock(pLobbyCreatedResponseMutex);

    if (bIOFailure)
    {
        /* Set the response to failed */
        lobbyCreatedResponse = E_ONLINE_SERVICE_RESPONSE_ERROR;
    }
    else
    {
        if (pLobby->m_eResult == k_EResultOK)
        {
            lobbyCreatedResponse = E_ONLINE_SERVICE_RESPONSE_SUCCESS;
            lobbyId = pLobby->m_ulSteamIDLobby;
        }
        else
        {
            lobbyCreatedResponse = E_ONLINE_SERVICE_RESPONSE_ERROR;
        }
    }

    /* Unlock the lobby created response variable */
    FMutexUnlock(pLobbyCreatedResponseMutex);
}

/* ====================================================== */ 
FOnlineService* FOnlineServiceCreate()
{
    FOnlineServiceSteam* pService = (FOnlineServiceSteam*)FAllocateZero(1, sizeof(FOnlineServiceSteam));
    if (pService == NULL)
    {
        return NULL;
    }

    return (FOnlineService*)pService;
}

/* ====================================================== */ 
void FOnlineServiceDestroy(FOnlineService** ppService)
{
    if (ppService == NULL || *ppService == NULL)
    {
        return;
    }

    FOnlineServiceSteam* pService = (FOnlineServiceSteam*)*ppService;

    SteamAPI_Shutdown();

    FDeallocate(pService);
    *ppService = NULL;
}

/* ====================================================== */
bool FOnlineServiceLogin(FOnlineService* pService, const char* pUsername, const char* pUserSecret)
{
    if (pService == NULL) /* Neither pUsername or pUserSecret are required for steam */
    {
        return false;
    }

    FOnlineServiceSteam* pServiceSteam = (FOnlineServiceSteam*)pService;

    if (!SteamAPI_Init())
    {
        return false;
    }

    pServiceSteam->pSteamUtils = SteamAPI_SteamUtils_v010();
    pServiceSteam->pSteamFriends = SteamAPI_SteamFriends_v017();
    pServiceSteam->pSteamUser = SteamAPI_SteamUser_v021();
    pServiceSteam->pSteamMatchmaking = SteamAPI_SteamMatchmaking_v009();

    if (pServiceSteam->pSteamUser == NULL || 
        pServiceSteam->pSteamUtils == NULL || 
        pServiceSteam->pSteamFriends == NULL ||
        pServiceSteam->pSteamMatchmaking == NULL)
    {
        return false;
    }

    return true;
}

/* ====================================================== */
bool FOnlineServiceIsLoggedIn(FOnlineService* pService, EOnlineServiceResponse* pResponse, FOnlineUserId* pUserId)
{
    if (pService == NULL || pResponse == NULL || pUserId == NULL)
    {   
        return false;
    }

    *pResponse = E_ONLINE_SERVICE_RESPONSE_ERROR;

    FOnlineServiceSteam* pServiceSteam = (FOnlineServiceSteam*)pService;
    if (pServiceSteam->pSteamUser == NULL)
    {
        return false;
    }

    if (!SteamAPI_ISteamUser_BLoggedOn(pServiceSteam->pSteamUser))
    {
        return false;
    }

    *pUserId = SteamAPI_ISteamUser_GetSteamID(pServiceSteam->pSteamUser);
    *pResponse = E_ONLINE_SERVICE_RESPONSE_SUCCESS;
    return true;
}

/* ====================================================== */
void FOnlineServiceFriendAdd(FOnlineService* pService, FOnlineUserId userToFriend)
{
    if (pService == NULL || userToFriend == 0)
    {
        return;
    }

    FOnlineServiceSteam* pServiceSteam = (FOnlineServiceSteam*)pService;

    SteamAPI_ISteamFriends_ActivateGameOverlayToUser(pServiceSteam->pSteamFriends, "friendadd", userToFriend);
}

/* ====================================================== */
void FOnlineServiceFriendRemove(FOnlineService* pService, FOnlineUserId userToRemove)
{
    if (pService == NULL || userToRemove == 0)
    {
        return;
    }

    FOnlineServiceSteam* pServiceSteam = (FOnlineServiceSteam*)pService;

    SteamAPI_ISteamFriends_ActivateGameOverlayToUser(pServiceSteam->pSteamFriends, "friendremove", userToRemove);
}

/* ====================================================== */
bool FOnlineServiceFriendCount(FOnlineService* pService, int64* pCount)
{
    if (pService == NULL || pCount == NULL)
    {
        return false;
    }

    FOnlineServiceSteam* pServiceSteam = (FOnlineServiceSteam*)pService;

    *pCount = SteamAPI_ISteamFriends_GetFriendCount(pServiceSteam->pSteamFriends, k_EFriendFlagAll);
    return true;
}

/* ====================================================== */
bool FOnlineServiceFriendAtIndex(FOnlineService* pService, int64 index, FOnlineUserId* pFriendId)
{
    if (pService == NULL || pFriendId == NULL)
    {
        return false;
    }

    FOnlineServiceSteam* pServiceSteam = (FOnlineServiceSteam*)pService;

    *pFriendId = SteamAPI_ISteamFriends_GetFriendByIndex(pServiceSteam->pSteamFriends, (int)index, k_EFriendFlagAll);
    return true;
}

/* ====================================================== */ 
bool FOnlineServiceLobbyCreate(FOnlineService* pService, EOnlineServiceLobbyType lobbyType, ui8 maxUsers)
{
    if (pService == NULL || maxUsers == 0)
    {
        return false;
    }

    FOnlineServiceSteam* pServiceSteam = (FOnlineServiceSteam*)pService;

    ELobbyType steamLobbyType = k_ELobbyTypePrivate;
    switch (lobbyType)
    {
        case E_ONLINE_SERVICE_LOBBY_TYPE_PRIVATE:
            steamLobbyType = k_ELobbyTypePrivate;
            break;
        case E_ONLINE_SERVICE_LOBBY_TYPE_FRIENDS:
            steamLobbyType = k_ELobbyTypeFriendsOnly;
            break;
        case E_ONLINE_SERVICE_LOBBY_TYPE_PUBLIC:
            steamLobbyType = k_ELobbyTypePublic;
            break;
        default:
            return false;
    }

    SteamAPICall_t createLobbyCall = SteamAPI_ISteamMatchmaking_CreateLobby(pServiceSteam->pSteamMatchmaking, steamLobbyType, maxUsers);
    if (createLobbyCall == 0)
    {
        return false;
    }

    /* Set the call result */
    pServiceSteam->m_SteamCallResultLobbyCreated.Set(createLobbyCall, pServiceSteam, &FOnlineServiceSteam::OnLobbyCreated);
    return true;
}

/* ====================================================== */
EOnlineServiceResponse FOnlineServiceLobbyCreatedResponse(FOnlineService* pService)
{
    if (pService == NULL)
    {
        return E_ONLINE_SERVICE_RESPONSE_ERROR;
    }

    FOnlineServiceSteam* pServiceSteam = (FOnlineServiceSteam*)pService;

    EOnlineServiceResponse response = E_ONLINE_SERVICE_RESPONSE_ERROR;

    /* Lock the pLobbyCreatedResponseMutex */
    FMutexLock(pServiceSteam->pLobbyCreatedResponseMutex);

    /* Check if the response is available */
    response = pServiceSteam->lobbyCreatedResponse;

    FMutexUnlock(pServiceSteam->pLobbyCreatedResponseMutex);

    return response;
}



void FOnlineServiceLobbyLeave(FOnlineService* pService)
{
    if (pService == NULL)
    {
        return;
    }

    FOnlineServiceSteam* pServiceSteam = (FOnlineServiceSteam*)pService;

    SteamAPI_ISteamMatchmaking_LeaveLobby(pServiceSteam->pSteamMatchmaking, pServiceSteam->lobbyId);
}

bool FOnlineServiceLobbyIsInLobby(FOnlineService* pService);
bool FOnlineServiceLobbyIsOwner(FOnlineService* pService);
bool FOnlineServiceLobbySetOwner(FOnlineService* pService, const FOnlineUserId newOwner); /* Must be lobby owner */
bool FOnlineServiceLobbySetData(FOnlineService* pService, const char* pName, const char* pValue); /* Must be lobby owner */
bool FOnlineServiceLobbyGetData(FOnlineService* pService, const char* pName, char* pValue, unsigned long valueMaxLength);
bool FOnlineServiceLobbyDeleteData(FOnlineService* pService, const char* pName); /* Must be lobby owner */


/* ====================================================== */ 
bool FOnlineServiceInviteFriendToLobby(FOnlineService* pService, FOnlineUserId friendToInvite)
{
    if (pService == NULL)
    {
        return false;
    }

    FOnlineServiceSteam* pServiceSteam = (FOnlineServiceSteam*)pService;

    if (!SteamAPI_ISteamFriends_InviteUserToGame(pServiceSteam->pSteamFriends, friendToInvite, NULL))
    {
        return false;
    }

    return true;
}

}