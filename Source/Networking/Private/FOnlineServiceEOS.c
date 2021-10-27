/* Copyright © 2021 Caden Miller, All Rights Reserved. */

#include <string.h>

#include <eos_sdk.h>
#include <eos_friends.h>
#include <eos_userinfo.h>

#include "mem.h"
#include "store.h"
#include "math.h"

#define EOSPRODUCTUSERID_TO_FSTOREID(id) (uint64_t)(uintptr_t)(id)
#define FSTOREID_TO_EOSPRODUCTUSERID(id) (uintptr_t)(id)

typedef struct FStoreEpicGames
{
    EOS_HPlatform platform;
    EOS_HConnect connect;
    EOS_ProductUserId userId;
    EOS_HFriends friends;
    EOS_HUserInfo userInfo;
    EOS_ProductUserId localUser;
    EOS_EpicAccountId localUserEpic;

    FStoreLoginCallback pLoginCallback;
} FStoreEpicGames;

typedef struct FStoreEpicGamesClientData
{
    FStoreEpicGames *pStore;
    FStoreCallback pCallback;
    void *pClientData;
    EOS_ProductUserId targetId;
} FStoreEpicGamesClientData;

void FStoreEpicGamesOnQueryProductUserIdMappings(const EOS_Connect_QueryProductUserIdMappingsCallbackInfo* pInfo)
{
    FStoreEpicGamesClientData* pClientData = (FStoreEpicGamesClientData*)pInfo->ClientData;
    FStoreQueryUserDataCallback pUserDataCallback = (FStoreQueryUserDataCallback)pClientData->pCallback;

    FStoreUserId targetId = EOSPRODUCTUSERID_TO_FSTOREID(pClientData->targetId);

    if (pClientData == NULL || pClientData->pCallback == NULL)
    {
        return;
    }

    if (pInfo->ResultCode != EOS_Success)
    {
        free(pClientData);
        return;
    }

    EOS_Connect_CopyProductUserExternalAccountByAccountIdOptions copyOptions = {0};
    copyOptions.ApiVersion = EOS_CONNECT_COPYPRODUCTUSEREXTERNALACCOUNTBYACCOUNTID_API_LATEST;
    copyOptions.TargetUserId = pClientData->targetId;
    copyOptions.AccountId = NULL;

    EOS_Connect_ExternalAccountInfo* pExternalAccountInfo = NULL;
    EOS_EResult result = EOS_Connect_CopyProductUserExternalAccountByAccountId(pClientData->pStore->connect, &copyOptions, &pExternalAccountInfo);

    if (result == EOS_NotFound)
    {
        /* Something went wrong */
        pUserDataCallback((FOnlineService*)pClientData->pStore, pClientData->pClientData, targetId, NULL);
        free(pClientData);
        return;
    }

    FStoreUserData userData = {0};
    FMemcpy(userData.pUserName, pExternalAccountInfo->DisplayName, strnlen(pExternalAccountInfo->DisplayName, EOS_USERINFO_MAX_DISPLAYNAME_CHARACTERS));
    userData.lastLogin = pExternalAccountInfo->LastLoginTime;

    pUserDataCallback((FOnlineService*)pClientData->pStore, pClientData->pClientData, targetId, &userData);
}

void FStoreEpicGamesOnLogin(const EOS_Connect_LoginCallbackInfo* pInfo)
{
    FStoreEpicGames* pStoreEpicGames = (FStoreEpicGames*)pInfo->ClientData;

    pStoreEpicGames->localUser = pInfo->LocalUserId
    
    EStoreLoginResponse loginResponse = E_STORE_LOGIN_RESPONSE_FAILED;
    switch(pInfo->ResultCode)
    {
        case EOS_Success:
            loginResponse = E_STORE_LOGIN_RESPONSE_LOGGED_IN;
            break;
        case EOS_TimedOut:
            loginResponse = E_STORE_LOGIN_RESPONSE_TIMEOUT;
            break;
        case EOS_Connect_ExternalTokenValidationFailed:
        case EOS_Connect_AuthExpired:
        case EOS_Connect_InvalidToken:
        case EOS_Connect_UnsupportedTokenType:
            loginResponse = E_STORE_LOGIN_RESPONSE_USERNAME_OR_USERSECRET_INCORRECT;
            break; 
        default:
            loginResponse = E_STORE_LOGIN_RESPONSE_FAILED;
    }

    if (pStoreEpicGames->pLoginCallback != NULL)
    {
        pStoreEpicGames->pLoginCallback((FOnlineService*)pStoreEpicGames, loginResponse, EOSPRODUCTUSERID_TO_FSTOREID(pInfo->LocalUserId));
    }
}

FResult FStoreCreate(FOnlineService** ppStore, const char* pUserName, const char* pUserSecret)
{
    if (ppStore == NULL)
    {
        return E_RESULT_FAILURE;
    }

    FStoreEpicGames* pStore = calloc(1, sizeof(FStoreEpicGames));
    if (pStore == NULL)
    {
        return E_RESULT_ERROR;
    }

    *ppStore = (FOnlineService*)pStore;

    /* Initialize the epic games store */
    EOS_InitializeOptions options = {0};
    options.ApiVersion = EOS_INITIALIZE_API_LATEST;
    options.ProductName = "EnterpriseEngine";
    options.ProductVersion = "0.1.0";

    if (EOS_Initialize(&options) != EOS_Success)
    {
        return E_RESULT_ERROR;
    }

    /* Create the platform */
    EOS_Platform_ClientCredentials clientCredentials = {0};
    //clientCredentials.ClientId = ENTERPRISE_EPIC_GAMES_CLIENT_ID;
    //clientCredentials.ClientSecret = ENTERPRISE_EPIC_GAMES_CLIENT_SECRET;

    EOS_Platform_Options platformOptions = {0};
    platformOptions.ApiVersion = EOS_PLATFORM_OPTIONS_API_LATEST;
    platformOptions.ProductId = ENTERPRISE_EPIC_GAMES_PRODUCT_ID;
    platformOptions.SandboxId = ENTERPRISE_EPIC_GAMES_SANDBOX_ID;
    platformOptions.DeploymentId = ENTERPRISE_EPIC_GAMES_DEPLOYMENT_ID;
    platformOptions.ClientCredentials = clientCredentials;

    #ifdef ENTERPRISE_EDITOR
    platformOptions.Flags = EOS_PF_LOADING_IN_EDITOR;
    #endif

    pStore->platform = EOS_Platform_Create(&platformOptions);

    if (pStore->platform == NULL)
    {
        return E_RESULT_ERROR;
    }

    /* Get the platform interfaces */
    pStore->connect = EOS_Platform_GetConnectInterface(pStore->platform);
    pStore->friends = EOS_Platform_GetFriendsInterface(pStore->platform);
    pStore->userInfo = EOS_Platform_GetUserInfoInterface(pStore->platform);

    if (pStore->connect == NULL ||
        pStore->friends == NULL ||
        pStore->userInfo == NULL)
    {
        return E_RESULT_ERROR;
    }

    /* Login to the connect interface */
    EOS_Connect_Credentials credentials = {0};
    credentials.ApiVersion = EOS_CONNECT_CREDENTIALS_API_LATEST;
    credentials.Token = pUserSecret;
    credentials.Type = EOS_ECT_EPIC;

    EOS_Connect_LoginOptions loginOptions = {0};
    loginOptions.ApiVersion = EOS_CONNECT_LOGIN_API_LATEST;
    loginOptions.Credentials = &credentials;
    loginOptions.UserLoginInfo = NULL;

    EOS_Connect_Login(pStore->connect, &loginOptions, pStore, FStoreEpicGamesOnLogin);
    return E_RESULT_SUCCESS;
}

void FStoreEpicGamesDestroy(FOnlineService* pStore)
{
    if (pStore == NULL)
    {
        return;
    }

    FStoreEpicGames* pStoreEpicGames = (FStoreEpicGames*)pStore;

    EOS_Platform_Release(pStoreEpicGames->platform);
    EOS_Shutdown();
}

FResult FStoreQueryUserData(FOnlineService* pStore, const FStoreUserId user, FStoreQueryUserDataCallback pCallback, void* pClientData)
{   
    if (pStore == NULL || pCallback == NULL)
    {
        return E_RESULT_FAILURE;
    }

    FStoreEpicGames* pStoreEpicGames = (FStoreEpicGames*)pStore;

    EOS_ProductUserId targetId = EOSPRODUCTUSERID_TO_FSTOREID(user);

    EOS_Connect_CopyProductUserExternalAccountByAccountIdOptions copyOptions = {0};
    copyOptions.ApiVersion = EOS_CONNECT_COPYPRODUCTUSEREXTERNALACCOUNTBYACCOUNTID_API_LATEST;
    copyOptions.TargetUserId = targetId;
    copyOptions.AccountId = NULL;

    EOS_Connect_ExternalAccountInfo* pExternalAccountInfo = NULL;
    EOS_EResult result = EOS_Connect_CopyProductUserExternalAccountByAccountId(pStoreEpicGames->connect, &copyOptions, &pExternalAccountInfo);

    /* The data was found so we can copy it */
    if (result == EOS_Success)
    {
        FStoreUserData userData = {0};
        FMemcpy(userData.pUserName, pExternalAccountInfo->DisplayName, strnlen(pExternalAccountInfo->DisplayName, EOS_USERINFO_MAX_DISPLAYNAME_CHARACTERS));

        /* Call the callback */
        pCallback(pStore, pClientData, user, &userData);

        return E_RESULT_SUCCESS;
    }

    /* If the data was not found we must query it */
    else if (result == EOS_NotFound)
    {
        EOS_Connect_QueryProductUserIdMappingsOptions queryOptions = {0};
        queryOptions.ApiVersion = EOS_CONNECT_QUERYPRODUCTUSERIDMAPPINGS_API_LATEST;
        queryOptions.LocalUserId = FSTOREID_TO_EOSPRODUCTUSERID(user);
        queryOptions.ProductUserIds = &targetId;
        queryOptions.ProductUserIdCount = 1;
        
        FStoreEpicGamesClientData* pStoreClientData = calloc(1, sizeof(FStoreEpicGamesClientData));
        pStoreClientData->pStore = pStoreEpicGames;
        pStoreClientData->pCallback = (FStoreCallback)pCallback;
        pStoreClientData->pClientData = pClientData;

        EOS_Connect_QueryProductUserIdMappings(pStoreEpicGames->connect, &queryOptions, pStoreClientData, FStoreEpicGamesOnQueryProductUserIdMappings);
        return E_RESULT_SUCCESS;
    }
    else
    {
        return E_RESULT_ERROR;
    }
}

FResult FStoreGetFriendsCount(FOnlineService* pStore, const FStoreUserId user, unsigned long* pCount)
{
    if (pStore == NULL || pCount == NULL)
    {
        return E_RESULT_FAILURE;
    }

    FStoreEpicGames* pStoreEpicGames = (FStoreEpicGames*)pStore;

    EOS_ProductUserId localUser = FSTOREID_TO_EOSPRODUCTUSERID(user);

    ProductUserIdTo

    EOS_Friends_GetFriendsCountOptions options = {0};
    options.ApiVersion = EOS_FRIENDS_GETFRIENDSCOUNT_API_LATEST;
    options.LocalUserId = localUser;

    int32_t count = EOS_Friends_GetFriendsCount(pStoreEpicGames->friends, &options);
    *pCount = (unsigned long)count;

    return E_RESULT_SUCCESS;
}

FResult FStoreGetFriendAtIndex(FOnlineService* pStore, const FStoreUserId user, unsigned long index, FStoreUserId* pFriend)
{

}

FResult FStoreEpicGamesGetUserIcon(FOnlineService* pStore, const FStoreUser user, const EStoreIconSize iconSize, unsigned long long dataSize, unsigned char* pData)
{

}