#include "Core/FLog.h"
#include "Core/FMemory.h"
#include "Core/FString.h"
#include "Core/FConfigParser.h"
#include "Core/FArgumentParser.h"
#include "Game/FGame.h"

FGame* FGameCreate(int argc, const char* const* argv)
{
    FGame* pGame = FAllocateZero(1, sizeof(FGame));
    if(pGame == NULL)
    {
        return NULL;
    }

    FLogInit();

    /* Parse the config files */
    FConfigOption pConfigOptions[] =
    {
        /* Render options */
        {"RenderAPI", "Vulkan", E_CONFIG_TYPE_STRING},
        {"RenderFullscreen", "True",  E_CONFIG_TYPE_BOOL},
        {"RenderWidth", "1080", E_CONFIG_TYPE_INT},
        {"RenderHeight", "720", E_CONFIG_TYPE_INT},
        {"RenderVSync", "False", E_CONFIG_TYPE_BOOL},
        {"RenderPhysicalDeviceVulkan", "0", E_CONFIG_TYPE_INT},

        /* Audio options */
        {"AudioAPI", "Unknown", E_CONFIG_TYPE_STRING},
        {"AudioVolumeMaster", "1.0", E_CONFIG_TYPE_FLOAT},
        {"AudioVolumeMusic", "1.0", E_CONFIG_TYPE_FLOAT},
        {"AudioVolumeSFX", "1.0", E_CONFIG_TYPE_FLOAT},
        {"AudioVolumeVoice", "1.0", E_CONFIG_TYPE_FLOAT},

        /* Lobby options */
        {"LobbyCreate", "False", E_CONFIG_TYPE_BOOL},
        {"LobbyConnect", "False", E_CONFIG_TYPE_BOOL},
        {"LobbyPort", "48230", E_CONFIG_TYPE_INT},
        {"LobbyMaxPlayers", "4", E_CONFIG_TYPE_INT},
        {"LobbyPassword", "", E_CONFIG_TYPE_STRING},
        {"LobbyIP", "192.168.1.1", E_CONFIG_TYPE_STRING},

        /* Dedicated server options */
        {"DedicatedServer", "False", E_CONFIG_TYPE_BOOL},
        {"DedicatedServerPort", "48230", E_CONFIG_TYPE_INT},
        {"DedicatedServerPassword", "", E_CONFIG_TYPE_STRING},
        {"DedicatedServerMaxPlayers", "4", E_CONFIG_TYPE_INT},
        {"DedicatedServerName", "UsersDedicatedServer", E_CONFIG_TYPE_STRING},
        {"DedicatedServerDescription", "This is the default server description.", E_CONFIG_TYPE_STRING},
        {"DedicatedServerGameMode", "Default", E_CONFIG_TYPE_INT},
        {"DedicatedServerOnlineSubsystem", "Steam", E_CONFIG_TYPE_STRING},
    };

    FConfigParser* pConfigParser = FConfigParserCreate(FCOUNT_OF(pConfigOptions), pConfigOptions);
    if (pConfigParser == NULL)
    {
        FGameDestroy(&pGame);
        return NULL;
    }
    
    if (!FConfigParserParse(pConfigParser, "Config/config.cfg"))
    {
        FGameDestroy(&pGame);
        return NULL;
    }

    /* Fill the different subsystems options with the config information */
    FRenderOptions renderOptions = {0};
    renderOptions.bDebugAndValidate = true;

    FConfigParsedOption configOptionRenderAPI = {0};
    /* FConfigParsedOption configOptionRenderFullscreen = {0}; */
    /* FConfigParsedOption configOptionRenderWidth = {0}; */
    /* FConfigParsedOption configOptionRenderHeight = {0}; */
    /* FConfigParsedOption configOptionRenderVSync = {0}; */
    /* FConfigParsedOption configOptionRenderPhysicalDeviceVulkan = {0}; */

    if (FConfigParserGetOption(pConfigParser, "RenderAPI", &configOptionRenderAPI))
    {
        if (FStringCompare(configOptionRenderAPI.value.String, ENTERPRISE_NAME_MAX_LENGTH, "Vulkan", sizeof("Vulkan")) == 0)
        {
            renderOptions.api = FRenderAPI;
        }
        else if (FStringCompare(configOptionRenderAPI.value.String, ENTERPRISE_NAME_MAX_LENGTH, "OpenGL", sizeof("OpenGL")) == 0)
        {
            renderOptions.api = E_RENDER_API_OPENGL;
        }
        else
        {
            FLogWarning("Config RenderAPI value was invalid, using default value.");
            FConfigParserResetOption(pConfigParser, "Config/config.cfg", "RenderAPI");
            FConfigParserGetOption(pConfigParser, "RenderAPI", &configOptionRenderAPI);
        }
    }

    FConfigParserDestroy(pConfigParser);

    /* Parse the args */
    FArgumentOption pArgumentOptions[] =
    {
        /* Online Service Options */
            /* EOS */
            {"AUTH_PASSWORD", "", E_ARGUMENT_TYPE_STRING},
            {"AUTH_TYPE", "", E_ARGUMENT_TYPE_STRING},

        /* Resource options */
        {"ResourceDefragment", " \"--ResourceDefragment=True\" Defragments the resource files to make them smaller.", E_ARGUMENT_TYPE_BOOL},

        /* Render options */
        {"RenderAPI", " \"--RenderAPI=Vulkan\" Forces the engine to run a specific render api.", E_ARGUMENT_TYPE_STRING},
        {"RenderFullscreen", " \"--RenderFullscreen\" Creates the main windows in fullscreen on startup.", E_ARGUMENT_TYPE_BOOL},
        {"RenderWidth", " \"--RenderWidth=1080\" Sets the main windows width on startup.", E_ARGUMENT_TYPE_INT},
        {"RenderHeight", " \"--RenderHeight=720\" Sets the main windows height on startup. ", E_ARGUMENT_TYPE_INT},
        {"RenderVSync", " \"--RenderVSync\" Enables VSync for the main window on startup. ", E_ARGUMENT_TYPE_BOOL},
        {"RenderPhysicalDeviceVulkan", " \"--RenderPhysicalDeviceVulkan\" When using the Vulkan api this forces a physical device to use.", E_ARGUMENT_TYPE_INT},

        /* Audio options */
        {"AudioAPI", " \"--AudioAPI=OpenAL\" Forces the engine to run a specific audio api.", E_ARGUMENT_TYPE_STRING},
        {"AudioVolumeMaster", " \"--AudioVolumeMaster=1.0\" Sets the master audio volume.", E_ARGUMENT_TYPE_FLOAT},
        {"AudioVolumeMusic", " \"--AudioVolumeMusic=1.0\" Sets the music audio volume.", E_ARGUMENT_TYPE_FLOAT},
        {"AudioVolumeSFX", " \"--AudioVolumeSFX=1.0\" Sets the sfx audio volume.", E_ARGUMENT_TYPE_FLOAT},
        {"AudioVolumeVoice", " \"--AudioVolumeVoice=1.0\" Sets the voice audio volume.", E_ARGUMENT_TYPE_FLOAT},

        /* Lobby options */
        {"LobbyCreate", " \"--LobbyStart\" Starts the lobby server on startup.", E_ARGUMENT_TYPE_BOOL},
        {"LobbyConnect", " \"--LobbyConnect\" Connects to the lobby server on startup.", E_ARGUMENT_TYPE_BOOL},
        {"LobbyPort", " \"--LobbyPort=48230\" Sets the lobby server port.", E_ARGUMENT_TYPE_INT},
        {"LobbyMaxPlayers", " \"--LobbyMaxPlayers=4\" Sets the maximum number of players in the lobby.", E_ARGUMENT_TYPE_INT},
        {"LobbyPassword", " \"--LobbyPassword=SuperSecretPassword\" Sets the lobby server password.", E_ARGUMENT_TYPE_STRING},
        {"LobbyIP", " \"--LobbyIP=127.0.0.1\" Sets the lobby server IP, not required for creating a lobby.", E_ARGUMENT_TYPE_STRING},

        /* Dedicated server options */
        {"DedicatedServer", " \"--DedicatedServer\" Enables a dedicated server and runs only in cli mode.", E_ARGUMENT_TYPE_BOOL},
        {"DedicatedServerPort", " \"--DedicatedServerPort=48230\" Sets the port for the dedicated server, this overrides config.", E_ARGUMENT_TYPE_INT},
        {"DedicatedServerPassword", " \"--DedicatedServerPassword=SuperSecretPassword\" Sets the password for the dedicated server, this overrides config.", E_ARGUMENT_TYPE_STRING},
        {"DedicatedServerMaxPlayers", " \"--DedicatedServerMaxPlayers=4\" Sets the max players for the dedicated server, this overrides config.", E_ARGUMENT_TYPE_INT},
        {"DedicatedServerName", " \"--DedicatedServerName=YourServerName\" Sets the name for the dedicated server, this overrides config.", E_ARGUMENT_TYPE_STRING},
        {"DedicatedServerDescription", " \"--DedicatedServerDescriptions=YourDescription\" Sets the description for the dedicated server, this overrides config.", E_ARGUMENT_TYPE_STRING},
        {"DedicatedServerGameMode", " \"--DedicatedServerGameMode=Default\" Sets the game mode for the dedicated server, this overrides config.", E_ARGUMENT_TYPE_INT},
        {"DedicatedServerOnlineSubsystem", " \"--DedicatedServerOnlineSubsystem=Steam\" Sets the online subsystem for the dedicated server, this overrides config. ", E_ARGUMENT_TYPE_STRING},
    };

    FArgumentParser* pArgumentParser = FArgumentParserCreate(FCOUNT_OF(pArgumentOptions), pArgumentOptions);
    if (pArgumentParser == NULL)
    {
        FGameDestroy(&pGame);
        return NULL;
    }
    
    FArgumentParserParse(pArgumentParser, argc, argv);

    const char* pOnlineServiceUsername = NULL;
    const char* pOnlineServiceSecret = NULL;

    /* Online Service arguments */
    FArgumentParsedOption argumentOptionAuthPassword = {0};
    FArgumentParsedOption argumentOptionAuthType = {0};

    if (FArgumentParserGetOption(pArgumentParser, "AUTH_TYPE", &argumentOptionAuthType))
    {
        pOnlineServiceUsername = argumentOptionAuthType.value.String;
    }

    if (FArgumentParserGetOption(pArgumentParser, "AUTH_PASSWORD", &argumentOptionAuthPassword))
    {
        pOnlineServiceSecret = argumentOptionAuthPassword.value.String;
    }

    /* Render API arguments */
    FArgumentParsedOption renderAPIOption = {0};
    FArgumentParsedOption renderFullscreenOption = {0};
    FArgumentParsedOption renderWidthOption = {0};
    FArgumentParsedOption renderHeightOption = {0};
    FArgumentParsedOption renderVSyncOption = {0};
    FArgumentParsedOption renderPhysicalDeviceVulkanOption = {0};
    
    if (FArgumentParserGetOption(pArgumentParser, "RenderAPI", &renderAPIOption))
    {
        if (FStringCompare(renderAPIOption.value.String, ENTERPRISE_NAME_MAX_LENGTH, "Vulkan", sizeof("Vulkan")) == 0)
        {
            renderOptions.api = E_RENDER_API_VULKAN;
        }
        else if (FStringCompare(renderAPIOption.value.String, ENTERPRISE_NAME_MAX_LENGTH, "OpenGL", sizeof("OpenGL")) == 0)
        {
            renderOptions.api = E_RENDER_API_OPENGL;
        }
        else
        {
            FLogWarning("Argument value for --RenderAPI was invalid, using config value.");
        }
    }

    if (FArgumentParserGetOption(pArgumentParser, "RenderFullscreen", &renderFullscreenOption))
    {
        renderOptions.bFullscreen = renderFullscreenOption.value.Bool;
    }

    if (FArgumentParserGetOption(pArgumentParser, "RenderWidth", &renderWidthOption))
    {
        renderOptions.width = renderWidthOption.value.Int;
    }

    if (FArgumentParserGetOption(pArgumentParser, "RenderHeight", &renderHeightOption))
    {
        renderOptions.height = renderHeightOption.value.Int;
    }

    if (FArgumentParserGetOption(pArgumentParser, "RenderVSync", &renderVSyncOption))
    {
        renderOptions.bVSync = renderVSyncOption.value.Bool;
    }

    if (FArgumentParserGetOption(pArgumentParser, "RenderPhysicalDeviceVulkan", &renderPhysicalDeviceVulkanOption))
    {
        renderOptions.physicalDeviceVulkanID = renderPhysicalDeviceVulkanOption.value.Int;
    }

    FArgumentParserDestroy(&pArgumentParser);



    pGame->pRenderDevice = FRendererCreate(&renderOptions);
    pGame->pOnlineService = FOnlineServiceCreate();

    if (pGame->pRenderDevice == NULL ||
        pGame->pOnlineService == NULL)
    {
        FGameDestroy(&pGame);
        return NULL;
    }

    if (!FOnlineServiceLogin(pGame->pOnlineService, pOnlineServiceUsername, pOnlineServiceSecret))
    {
        FGameDestroy(&pGame);
    }

    return pGame;
}

void FGameDestroy(FGame** ppGame)
{   
    if (ppGame == NULL)
    {
        return;
    }

    FGame* pGame = *ppGame;

    FRendererDestroy(&pGame->pRenderDevice);
    FOnlineServiceDestroy(&pGame->pOnlineService);
    
    FLogShutdown();

    FDeallocate(pGame);
    *ppGame = NULL;
}

void FGameRun(FGame* pGame)
{
    if (pGame == NULL)
    {
        return;
    }
}