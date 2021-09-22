set(WindowSources "")

if (CMAKE_SYSTEM_NAME STREQUAL "Windows")
    list(APPEND WindowSources
        "Private/FWindowWindows.c")
elseif(CMAKE_SYSTEM_NAME STREQUAL "Darwin")
    list(APPEND WindowSources
        "Private/FWindowMacOS.c")
elseif(CMAKE_SYSTEM_NAME STREQUAL "Android")
    list(APPEND WindowSources
        "Private/FWindowAndroid.c")
elseif(CMAKE_SYSTEM_NAME STREQUAL "Linux")
    list(APPEND WindowSources
        "Private/FWindowLinuxX11.c")
endif()

add_library(EnterpriseWindow SHARED ${WindowSources})
target_include_directories(EnterpriseWindow PUBLIC "Public")

target_link_libraries(EnterpriseWindow EnterpriseCore)

