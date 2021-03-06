
# Find the third party libs
find_package(Vulkan REQUIRED)

set(GraphicsSources "")

list(APPEND GraphicsSources
    "Private/Mesh/enMeshParser.c"
    "Private/Mesh/enMeshParserGLTF.c")

if (CMAKE_SYSTEM_NAME STREQUAL "Windows")
    list(APPEND GraphicsSources
        "Private/Window/enWindowWindows.c")
elseif(CMAKE_SYSTEM_NAME STREQUAL "Darwin")
    list(APPEND GraphicsSources
        "Private/Window/enWindowMacOS.c")
elseif(CMAKE_SYSTEM_NAME STREQUAL "Android")
    list(APPEND GraphicsSources
        "Private/Window/enWindowAndroid.c")
elseif(CMAKE_SYSTEM_NAME STREQUAL "Linux")
    list(APPEND GraphicsSources
        "Private/Window/enWindowXcb.c")
endif()

# Vulkan API
if (Vulkan_FOUND)
    list(APPEND GraphicsSources
        "Private/Vulkan/enVulkanExtensions.c"    
        "Private/Vulkan/enVulkanFormats.c"
        "Private/Vulkan/enVulkanMemoryAllocator.cpp"
        "Private/Vulkan/enVulkanBuffer.c" 
        "Private/Vulkan/enWindowVulkan.c"
        "Private/Vulkan/enGraphicsVulkan.c"
        "Private/Vulkan/enImageVulkan.c"
        "Private/Vulkan/enPipelineVulkan.c")
endif()

add_library(EnterpriseGraphics STATIC ${GraphicsSources})
target_include_directories(EnterpriseGraphics PUBLIC "Public")
target_link_libraries(EnterpriseGraphics EnterpriseCore EnterpriseResource)

# Include ThirdParty Libraries
if(CMAKE_SYSTEM_NAME STREQUAL "Darwin")
    list(APPEND GraphicsSources
        "Private/Window/enWindowMacOS.c")
elseif(CMAKE_SYSTEM_NAME STREQUAL "Android")
    list(APPEND GraphicsSources
        "Private/Window/enWindowAndroid.c")
elseif(CMAKE_SYSTEM_NAME STREQUAL "Linux")
    target_link_libraries(EnterpriseGraphics xcb xcb-icccm)
endif()

if (Vulkan_FOUND)
    target_include_directories(EnterpriseGraphics PUBLIC ${Vulkan_INCLUDE_DIR})
    target_link_libraries(EnterpriseGraphics ${Vulkan_LIBRARY})
    target_compile_definitions(EnterpriseGraphics PUBLIC ENTERPRISE_GRAPHICS_VULKAN)

    target_link_libraries(EnterpriseGraphics VulkanMemoryAllocator)
endif()