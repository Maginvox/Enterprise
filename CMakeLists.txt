cmake_minimum_required(VERSION 3.12)
project(Enterprise)

# Compile the third party libraries
add_subdirectory(ThirdParty)

SET(ENTERPRISE_DIR "${CMAKE_SOURCE_DIR}")

# Set the prebuilt libraries that required authorisation from the companys
set(ENTERPRISE_STEAMWORKS_ENABLED OFF CACHE BOOL "")
set(ENTERPRISE_STEAMWORKS_PATH "${ENTERPRISE_DIR}/ThirdParty/Steamworks/sdk")
set(ENTERPRISE_STEAMWORKS_LIBRARY_PATH "${ENTERPRISE_STEAMWORKS_PATH}/redistributable_bin")
set(ENTERPRISE_STEAMWORKS_INCLUDE_PATH "${ENTERPRISE_STEAMWORKS_PATH}/public")
set(ENTERPRISE_EPIC_GAMES_ENABLED OFF CACHE BOOL "")
set(ENTERPRISE_EPIC_GAMES_PATH "${ENTERPRISE_DIR}/ThirdParty/EpicOnlineServices/SDK")
set(ENTERPRISE_EPIC_GAMES_LIBRARY_PATH "${ENTERPRISE_EPIC_GAMES_PATH}")
set(ENTERPRISE_EPIC_GAMES_INCLUDE_PATH "${ENTERPRISE_EPIC_GAMES_PATH}/Include")
set(ENTERPRISE_EPIC_GAMES_PRODUCT_ID "" CACHE STRING "")
set(ENTERPRISE_EPIC_GAMES_SANDBOX_ID "" CACHE STRING "")
set(ENTERPRISE_EPIC_GAMES_DEPLOYMENT_ID "" CACHE STRING "")
set(ENTERPRISE_DISCORD_ENABLED OFF CACHE BOOL "")
set(ENTERPRISE_DISCORD_PATH "${ENTERPRISE_DIR}/ThirdParty/discord_game_sdk")
set(ENTERPRISE_DISCORD_LIBRARY_PATH "${ENTERPRISE_DISCORD_PATH}/lib/x86_64")
set(ENTERPRISE_DISCORD_INCLUDE_PATH "${ENTERPRISE_DISCORD_PATH}/c")
set(ENTERPRISE_BUILD_TESTS OFF CACHE BOOL "")
set(ENTERPRISE_BUILD_STATIC ON CACHE BOOL "") # If not it will build shared library

if (ENTERPRISE_STEAMWORKS_ENABLED MATCHES ON)
    message(STATUS "Steamworks SDK enabled")
    message(STATUS "Steamworks SDK path: ${ENTERPRISE_STEAMWORKS_PATH}")
    message(STATUS "Steamworks SDK library path: ${ENTERPRISE_STEAMWORKS_LIBRARY_PATH}")
    message(STATUS "Steamworks SDK include path: ${ENTERPRISE_STEAMWORKS_INCLUDE_PATH}")
elseif (ENTERPRISE_EPIC_GAMES_ENABLED)
    message(STATUS "Epic Games SDK enabled")
    message(STATUS "Epic Games SDK path: ${ENTERPRISE_EPIC_GAMES_PATH}")
    message(STATUS "Epic Games SDK library path: ${ENTERPRISE_EPIC_GAMES_LIBRARY_PATH}")
    message(STATUS "Epic Games SDK include path: ${ENTERPRISE_EPIC_GAMES_INCLUDE_PATH}")
elseif (ENTERPRISE_DISCORD_ENABLED)
    message(STATUS "Discord Game SDK enabled")
    message(STATUS "Discord Game SDK path: ${ENTERPRISE_DISCORD_PATH}")
    message(STATUS "Discord Game SDK library path: ${ENTERPRISE_DISCORD_LIBRARY_PATH}")
    message(STATUS "Discord Game SDK include path: ${ENTERPRISE_DISCORD_INCLUDE_PATH}")
endif()


# Compile the engine
add_subdirectory(Source)

if (ENTERPRISE_BUILD_TESTS)
    add_subdirectory(Tests)
endif()