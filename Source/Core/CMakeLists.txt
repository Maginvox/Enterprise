set (CoreSources "")

list(APPEND CoreSources
    "FMemory.c"
    "FString.c"
    "FArray.c"
    "FList.c"
    "FTime.c"
    "FThread.c"
    "FHash.c"
    "FHashMap.c"
    "FArgumentParser.c"
    "FConfigParser.c")

add_library(EnterpriseCore SHARED ${CoreSources})
target_include_directories(EnterpriseCore PUBLIC "${PROJECT_SOURCE_DIR}/Include/")