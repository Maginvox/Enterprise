
# Find the required third party libraries
find_package(Vulkan REQUIRED)


set (EnterpriseSources "")

# Add the Core sources
list (APPEND EnterpriseSources 
	"Core/FMemory.c"
	"Core/FString.c"
	"Core/FArray.c"
	"Core/FTime.c"
	"Core/FThread.c"
	"Core/FHash.c"
	"Core/FHashMap.c"
	"Core/FArgumentParser.c"
	"Core/FConfigParser.c")

# Add the IO sources
list (APPEND EnterpriseSources
	"IO/FFile.c"
	"IO/FLog.c")

# Add the Networking sources
list (APPEND EnterpriseSources
	"Networking/FSocket.c")

if (ENTERPRISE_STEAMWORKS_ENABLED)
	message("Networking: Using Steamworks Service")	
	list (APPEND EnterpriseSources
		"Networking/FOnlineServiceSteam.cpp")
elseif (ENTERPRISE_EPIC_GAMES_ENABLED)
	message("Networking: Using Epic Online Service")	
	list (APPEND EnterpriseSources
		"Networking/FOnlineServiceEOS.c")
elseif (ENTERPRISE_DISCORD_ENABLED)
	message("Networking: Using Discord Service")	
	list (APPEND EnterpriseSources
		"Networking/FOnlineServiceDiscord.c")
else()
	message("Networking: Using Null Service")
	list (APPEND EnterpriseSources
		"Networking/FOnlineServiceNull.c")
endif()

# Add the Graphics sources
add_subdirectory(Graphics)

# Add the Resource sources
list (APPEND EnterpriseSources
	"Resource/FPackage.c")

# Add the Game sources
list (APPEND EnterpriseSources
	"Game/FGame.c")

# Create the Enterprise library
if (ENTERPRISE_BUILD_STATIC)
	add_library(Enterprise STATIC ${EnterpriseSources})
else()
	add_library(Enterprise SHARED ${EnterpriseSources})
endif()

# Set the properties
set_property(TARGET Enterprise PROPERTY C_STANDARD 99)

if (MSVC)
	# warning level 4 and all warnings as errors
	target_compile_options(Enterprise PRIVATE /W4)
else()
	# lots of warnings and all warnings as errors
	target_compile_options(Enterprise PRIVATE -Wall -Wextra -Werror=pedantic )
endif()

# Setup the include paths
target_include_directories(Enterprise PUBLIC "${PROJECT_SOURCE_DIR}/Include")
target_include_directories(Enterprise PRIVATE ${Vulkan_INCLUDE_DIR})

if (ENTERPRISE_STEAMWORKS_ENABLED)
	target_include_directories(Enterprise PRIVATE ${ENTERPRISE_STEAMWORKS_INCLUDE_PATH})
elseif (ENTERPRISE_EPIC_GAMES_ENABLED)
	target_include_directories(Enterprise PRIVATE ${ENTERPRISE_EPIC_GAMES_INCLUDE_PATH})
elseif (ENTERPRISE_DISCORD_ENABLED)
	target_include_directories(Enterprise PRIVATE ${ENTERPRISE_DISCORD_INCLUDE_PATH})
endif()

# Link the third party libraries
target_link_libraries(Enterprise ${Vulkan_LIBRARIES})
target_link_libraries(Enterprise SDL2)
target_link_libraries(Enterprise lz4)
target_link_libraries(Enterprise VulkanMemoryAllocator)

if (ENTERPRISE_STEAMWORKS_ENABLED)
	if (CMAKE_SYSTEM_NAME STREQUAL "Windows")
		target_link_libraries(Enterprise "${ENTERPRISE_STEAMWORKS_LIBRARY_PATH}/win64/steam_api64.lib")
	elseif (CMAKE_SYSTEM_NAME STREQUAL "Linux")
		target_link_libraries(Enterprise "${ENTERPRISE_STEAMWORKS_LIBRARY_PATH}/linux64/libsteam_api.so")
	endif()

elseif (ENTERPRISE_EPIC_GAMES_ENABLED)
	target_link_libraries(Enterprise ${ENTERPRISE_EPIC_GAMES_LIBRARY_PATH})
	if (CMAKE_SYSTEM_NAME STREQUAL "Windows")
		target_link_libraries(Enterprise "${ENTERPRISE_EPIC_GAMES_LIBRARY_PATH}/Lib/EOSSDK-Win64-Shipping.lib")
	elseif (CMAKE_SYSTEM_NAME STREQUAL "Linux")
		target_link_libraries(Enterprise "${ENTERPRISE_EPIC_GAMES_LIBRARY_PATH}/Bin/EOSSDK-Linux-Shipping.so")
	endif()
	
elseif (ENTERPRISE_DISCORD_ENABLED)
	if (CMAKE_SYSTEM_NAME STREQUAL "Windows")
		target_link_libraries(Enterprise "${ENTERPRISE_DISCORD_LIBRARY_PATH}/discord_game_sdk.dll.lib")
	elseif (CMAKE_SYSTEM_NAME STREQUAL "Linux")
		target_link_libraries(Enterprise "${ENTERPRISE_DISCORD_LIBRARY_PATH}/discord_game_sdk.so")
	endif()
endif()

# Add the compile definitions
set(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -DFDEBUG")

if (CMAKE_SYSTEM_NAME STREQUAL "Windows")
	target_compile_definitions(Enterprise PRIVATE ENTERPRISE_WINDOWS)
elseif (CMAKE_SYSTEM_NAME STREQUAL "Linux")
	target_compile_definitions(Enterprise PRIVATE ENTERPRISE_LINUX)
endif()

if (Vulkan_FOUND)
	target_compile_definitions(Enterprise PRIVATE ENTERPRISE_VULKAN)
endif()

if (CMAKE_SYSTEM_NAME STREQUAL "Windows")
	target_compile_definitions(Enterprise PRIVATE ENTERPRISE_D3D12)
endif()

if (ENTERPRISE_STEAMWORKS_ENABLED)
	target_compile_definitions(Enterprise PRIVATE ENTERPRISE_ONLINE_SERVICE_STEAMWORKS)
elseif (ENTERPRISE_EPIC_GAMES_ENABLED)
	target_compile_definitions(Enterprise PRIVATE ENTERPRISE_ONLINE_SERVICE_EPIC_GAMES)
endif()
if (ENTERPRISE_DISCORD_ENABLED)
	target_compile_definitions(Enterprise PRIVATE ENTERPRISE_ONLINE_SERVICE_DISCORD)
endif()