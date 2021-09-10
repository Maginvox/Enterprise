set(ResourceSources "")

list(APPEND ResourceSources
    "FPackage.c")

add_library(EnterpriseResource SHARED ${ResourceSources})
target_include_directories(EnterpriseResource PUBLIC "${PROJECT_SOURCE_DIR}/Include/")

target_link_libraries(EnterpriseResource EnterpriseCore)