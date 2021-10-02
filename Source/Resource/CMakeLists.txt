set(ResourceSources "")

list(APPEND ResourceSources
    "Private/FPackage.c")

add_library(EnterpriseResource STATIC ${ResourceSources})
target_include_directories(EnterpriseResource PUBLIC "Public")

target_link_libraries(EnterpriseResource EnterpriseCore)