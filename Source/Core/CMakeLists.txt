set (CoreSources "")

list(APPEND CoreSources
    "Private/FFile.c"
    "Private/FMemory.c"
    "Private/FString.c"
    "Private/FLog.c"
    "Private/FArray.c"
    "Private/FList.c"
    "Private/FTime.c"
    "Private/FThread.c"
    "Private/FHash.c"
    "Private/FHashMap.c"
    "Private/FArgumentParser.c"
    "Private/FConfigParser.c")

add_library(EnterpriseCore SHARED ${CoreSources})
target_include_directories(EnterpriseCore PUBLIC "Public")

include(GenerateExportHeader)
generate_export_header(EnterpriseCore EXPORT_FILE_NAME Public/Export.h)