set (CoreSources "")

list(APPEND CoreSources
		"Private/enFile.c"
		"Private/enMemory.c"
		"Private/enString.c"
		"Private/enLog.c"
		"Private/enArray.c"
		"Private/enList.c"
		"Private/enTime.c"
		"Private/enThread.c"
		"Private/enHash.c"
		"Private/enHashMap.c"
		"Private/enMD5.c"
		"Private/enArgumentParser.c"
		"Private/enConfigParser.c"
		"Private/enJsmn.c")

add_library(EnterpriseCore STATIC ${CoreSources})
target_include_directories(EnterpriseCore PUBLIC "Public")