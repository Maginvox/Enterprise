#include "Networking/FOnlineService.h"


FOnlineService* FOnlineServiceCreate(); 
void FOnlineServiceDestroy(FOnlineService** pService);

bool FOnlineServiceLogin(FOnlineService* pService, const char* pUsername, const char* pUserSecret)
{
	return false;
}

void FOnlineServiceLoginIsLoggedIn(FOnlineService* pService, EOnlineServiceResponse* pResponse, FOnlineUserId* pUserId)
{

}

void FOnlineServiceFriendAdd(FOnlineService* pService, const FOnlineUserId userToFriend)
{

}

void FOnlineServiceFriendRemove(FOnlineService* pService, const FOnlineUserId user)
{

}

bool FOnlineServiceFriendCount(FOnlineService* pService, FInt64* pCount)
{
	return false;
}

bool FOnlineServiceFriendAtIndex(FOnlineService* pService, FInt64 index, FOnlineUserId* pFriendId)
{
	return false;
}

bool FOnlineServiceLobbyCreate(FOnlineService* pService, const EOnlineServiceLobbyType lobbyType, const FUInt8 maxUsers)
{
	return false;
}

EOnlineServiceResponse FOnlineServiceLobbyCreatedResponse(FOnlineService* pService)
{
	return E_ONLINE_SERVICE_RESPONSE_SUCCESS;
}

void FOnlineServiceLobbyLeave(FOnlineService* pService)
{

}

bool FOnlineServiceLobbyIsInLobby(FOnlineService* pService)
{
	return false;
}

bool FOnlineServiceLobbyIsOwner(FOnlineService* pService)
{
	return false;
}

bool FOnlineServiceLobbySetOwner(FOnlineService* pService, const FOnlineUserId newOwner)
{
	return false;
}

bool FOnlineServiceLobbySetData(FOnlineService* pService, const char* pName, const char* pValue)
{
	return false;
}

bool FOnlineServiceLobbyGetData(FOnlineService* pService, const char* pName, char* pValue, unsigned long valueMaxLength)
{
	return false;
}

bool FOnlineServiceLobbyDeleteData(FOnlineService* pService, const char* pName)
{
	return false;
}

bool FOnlineServiceInviteToLobby(FOnlineService* pService, const FOnlineUserId friendToInvite)
{
	return false;
}

void FOnlineServiceInviteAccept(FOnlineService* pService, const FOnlineUserId friendInviteFrom)
{

}

void FOnlineServiceInviteDeny(FOnlineService* pService, const FOnlineUserId friendInviteFrom)
{

}

void FOnlineServiceRichPresenceUpdate(FOnlineService* pService, const char* pRichPresence)
{

}

void FOnlineServiceRichPresenceClear(FOnlineService* pService)
{

}
