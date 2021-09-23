set(WindowSources "")

if (CMAKE_SYSTEM_NAME STREQUAL "Windows")
    list(APPEND WindowSources
        "Private/FWindowWindows.c"
        "Private/FInputWindows.c")
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

# Build both shared and static
add_library(EnterpriseWindow STATIC ${WindowSources})

target_include_directories(EnterpriseWindow PUBLIC "Public")

target_link_libraries(EnterpriseWindow EnterpriseCore)

