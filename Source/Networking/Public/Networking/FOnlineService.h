#ifndef __FSTORE_H__
#define __FSTORE_H__

#include "FTypes.h"

#define ONLINE_SERVICE_USERNAME_MAX_LENGTH 128 /* This must be the highest value out of the online service username limit */

typedef struct FOnlineService FOnlineService;

typedef ui64 FOnlineUserId;

typedef enum EOnlineServiceResponse
{
    E_ONLINE_SERVICE_RESPONSE_ERROR = 0,
    E_ONLINE_SERVICE_RESPONSE_SUCCESS = 1,
    E_ONLINE_SERVICE_RESPONSE_WAITING = 2,
} EOnlineServiceResponse;

typedef enum EOnlineServiceLobbyType {
    E_ONLINE_SERVICE_LOBBY_TYPE_PRIVATE = 0,
    E_ONLINE_SERVICE_LOBBY_TYPE_FRIENDS = 1,
    E_ONLINE_SERVICE_LOBBY_TYPE_PUBLIC = 2
} EOnlineServiceLobbyType;

typedef struct FOnlineServiceUserData
{
    char username[ONLINE_SERVICE_USERNAME_MAX_LENGTH];
} FOnlineServiceUserData;

typedef struct FOnlineServiceLobbyData
{
    EOnlineServiceLobbyType lobbyType;
    FOnlineServiceUserData lobbyOwner;
    int32 gameIp;
} FOnlineServiceLobbyData;

FOnlineService* FOnlineServiceCreate(); 
void FOnlineServiceDestroy(FOnlineService** pService);

bool FOnlineServiceLogin(FOnlineService* pService, const char* pUsername, const char* pUserSecret); /* EOS requires command line authentication details */
void FOnlineServiceLoginIsLoggedIn(FOnlineService* pService, EOnlineServiceResponse* pResponse, FOnlineUserId* pUserId); /* Check this before running any other online service functions */

void FOnlineServiceFriendAdd(FOnlineService* pService, const FOnlineUserId userToFriend);
void FOnlineServiceFriendRemove(FOnlineService* pService, const FOnlineUserId user);
bool FOnlineServiceFriendCount(FOnlineService* pService, int64* pCount);
bool FOnlineServiceFriendAtIndex(FOnlineService* pService, int64 index, FOnlineUserId* pFriendId);

bool FOnlineServiceLobbyCreate(FOnlineService* pService, const EOnlineServiceLobbyType lobbyType, const ui8 maxUsers); /* Get the new FOnlineServiceLobbyId from the FOnlineServiceLobbyCreatedCallback */
EOnlineServiceResponse FOnlineServiceLobbyCreatedResponse(FOnlineService* pService);
void FOnlineServiceLobbyLeave(FOnlineService* pService);
bool FOnlineServiceLobbyIsInLobby(FOnlineService* pService);
bool FOnlineServiceLobbyIsOwner(FOnlineService* pService);
bool FOnlineServiceLobbySetOwner(FOnlineService* pService, const FOnlineUserId newOwner); /* Must be lobby owner */
bool FOnlineServiceLobbySetData(FOnlineService* pService, const char* pName, const char* pValue); /* Must be lobby owner */
bool FOnlineServiceLobbyGetData(FOnlineService* pService, const char* pName, char* pValue, unsigned long valueMaxLength);
bool FOnlineServiceLobbyDeleteData(FOnlineService* pService, const char* pName); /* Must be lobby owner */

bool FOnlineServiceInviteToLobby(FOnlineService* pService, const FOnlineUserId friendToInvite);
void FOnlineServiceInviteAccept(FOnlineService* pService, const FOnlineUserId friendInviteFrom);
void FOnlineServiceInviteDeny(FOnlineService* pService, const FOnlineUserId friendInviteFrom);

void FOnlineServiceRichPresenceUpdate(FOnlineService* pService, const char* pRichPresence);
void FOnlineServiceRichPresenceClear(FOnlineService* pService);

#endif
