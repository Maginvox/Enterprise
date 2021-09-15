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

if (CMAKE_SYSTEM_NAME STREQUAL "Windows")
	target_compile_definitions(EnterpriseCore PUBLIC ENTERPRISE_CORE_WINDOWS)
elseif (CMAKE_SYSTEM_NAME STREQUAL "Darwin")
	target_compile_definitions(EnterpriseCore PUBLIC ENTERPRISE_CORE_MACOS)
elseif (CMAKE_SYSTEM_NAME STREQUAL "Linux")
	target_compile_definitions(EnterpriseCore PUBLIC ENTERPRISE_CORE_LINUX)
endif()