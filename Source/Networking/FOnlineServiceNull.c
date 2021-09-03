#include "Networking/FOnlineService.h"


FOnlineService* FOnlineServiceCreate()
{
	return NULL;
}

void FOnlineServiceDestroy(FOnlineService** ppService)
{
	if (ppService == NULL)
	{
		return;
	}
}

bool FOnlineServiceLogin(FOnlineService* pService, const char* pUsername, const char* pUserSecret)
{
	if (pService == NULL || pUsername == NULL || pUserSecret == NULL)
	{
		return false;
	}
	return false;
}

void FOnlineServiceLoginIsLoggedIn(FOnlineService* pService, EOnlineServiceResponse* pResponse, FOnlineUserId* pUserId)
{
	if (pService == NULL || pResponse == NULL || pUserId == NULL)
	{
		return;
	}
}

void FOnlineServiceFriendAdd(FOnlineService* pService, const FOnlineUserId userToFriend)
{
	if (pService == NULL || userToFriend == 0)
	{
		return;
	}
}

void FOnlineServiceFriendRemove(FOnlineService* pService, const FOnlineUserId user)
{
	if (pService == NULL || user == 0)
	{
		return;
	}
}

bool FOnlineServiceFriendCount(FOnlineService* pService, FInt64* pCount)
{
	if (pService == NULL || pCount == NULL)
	{
		return false;
	}
	return false;
}

bool FOnlineServiceFriendAtIndex(FOnlineService* pService, FInt64 index, FOnlineUserId* pFriendId)
{
	if (pService == NULL || index == 0 || pFriendId == NULL)
	{
		return false;
	}
	return false;
}

bool FOnlineServiceLobbyCreate(FOnlineService* pService, const EOnlineServiceLobbyType lobbyType, const FUInt8 maxUsers)
{
	if (pService == NULL || lobbyType == 0 || maxUsers == 0)
	{
		return false;
	}
	return false;
}

EOnlineServiceResponse FOnlineServiceLobbyCreatedResponse(FOnlineService* pService)
{
	if (pService == NULL)
	{
		return E_ONLINE_SERVICE_RESPONSE_ERROR;
	}
	return E_ONLINE_SERVICE_RESPONSE_ERROR;
}

void FOnlineServiceLobbyLeave(FOnlineService* pService)
{
	if (pService == NULL)
	{
		return;
	}
}

bool FOnlineServiceLobbyIsInLobby(FOnlineService* pService)
{
	if (pService == NULL)
	{
		return false;
	}	
	return false;
}

bool FOnlineServiceLobbyIsOwner(FOnlineService* pService)
{
	if (pService == NULL)
	{
		return false;
	}
	return false;
}

bool FOnlineServiceLobbySetOwner(FOnlineService* pService, const FOnlineUserId newOwner)
{
	if (pService == NULL || newOwner == 0)
	{
		return false;
	}
	return false;
}

bool FOnlineServiceLobbySetData(FOnlineService* pService, const char* pName, const char* pValue)
{
	if (pService == NULL || pName == NULL || pValue == NULL)
	{
		return false;
	}
	return false;
}

bool FOnlineServiceLobbyGetData(FOnlineService* pService, const char* pName, char* pValue, unsigned long valueMaxLength)
{
	if (pService == NULL || pName == NULL || pValue == NULL || valueMaxLength == 0)
	{
		return false;
	}
	return false;
}

bool FOnlineServiceLobbyDeleteData(FOnlineService* pService, const char* pName)
{
	if (pService == NULL || pName == NULL)
	{
		return false;
	}
	return false;
}

bool FOnlineServiceInviteToLobby(FOnlineService* pService, const FOnlineUserId friendToInvite)
{
	if (pService == NULL || friendToInvite == 0)
	{
		return false;
	}
	return false;
}

void FOnlineServiceInviteAccept(FOnlineService* pService, const FOnlineUserId friendInviteFrom)
{
	if (pService == NULL || friendInviteFrom == 0)
	{
		return;
	}
}

void FOnlineServiceInviteDeny(FOnlineService* pService, const FOnlineUserId friendInviteFrom)
{
	if (pService == NULL || friendInviteFrom == 0)
	{
		return;
	}
}

void FOnlineServiceRichPresenceUpdate(FOnlineService* pService, const char* pRichPresence)
{
	if (pService == NULL || pRichPresence == NULL)
	{
		return;
	}
}

void FOnlineServiceRichPresenceClear(FOnlineService* pService)
{
	if (pService == NULL)
	{
		return;
	}

}
