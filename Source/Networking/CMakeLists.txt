set(NetworkingSources "")

list(APPEND NetworkingSources
    "FSocket.c")

if (ENTEPRISE_NETWORKING_HAS_STEAMWORKS)
    list(APPEND NetworkingSources
        "FOnlineServiceSteam.cpp")

elseif(ENTERPRISE_NETWORKING_HAS_EPICGAMES)
    list(APPEND NetworkingSources
        "FOnlineServiceEOS.c")
endif()

if(ENTERPRISE_NETWORKING_HAS_DISCORD)
    
endif()

add_library(EnterpriseNetworking STATIC ${NetworkingSources})
target_link_libraries(EnterpriseNetworking EnterpriseCore)
target_include_directories(EnterpriseNetworking PUBLIC "${PROJECT_SOURCE_DIR}/Include/")