set(InputSources "")

if (CMAKE_SYSTEM_NAME STREQUAL "Windows")
    list(APPEND InputSources
        "Private/enInputWindows.c")
elseif(CMAKE_SYSTEM_NAME STREQUAL "Darwin")

elseif(CMAKE_SYSTEM_NAME STREQUAL "Android")

elseif(CMAKE_SYSTEM_NAME STREQUAL "Linux")
    list(APPEND InputSources
        "Private/enInputXcb.c")
endif()

add_library(EnterpriseInput STATIC ${InputSources})
target_include_directories(EnterpriseInput PUBLIC "Public")

target_link_libraries(EnterpriseInput EnterpriseGraphics)