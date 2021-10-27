set(NetworkingSources "")

list(APPEND NetworkingSources
    "Private/FSocket.c")

if (ENTEPRISE_NETWORKING_HAS_STEAMWORKS)
    list(APPEND NetworkingSources
        "Private/FOnlineServiceSteam.cpp")

elseif(ENTERPRISE_NETWORKING_HAS_EPICGAMES)
    list(APPEND NetworkingSources
        "Private/FOnlineServiceEOS.c")
endif()

if(ENTERPRISE_NETWORKING_HAS_DISCORD)
    
endif()

add_library(EnterpriseNetworking STATIC ${NetworkingSources})
target_link_libraries(EnterpriseNetworking EnterpriseCore)
target_include_directories(EnterpriseNetworking PUBLIC "Public")