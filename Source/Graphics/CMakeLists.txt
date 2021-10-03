
# Find the third party libs
find_package(Vulkan REQUIRED)

set(GraphicsSources "")


if (CMAKE_SYSTEM_NAME STREQUAL "Windows")
    list(APPEND GraphicsSources
        "Private/Window/FWindowWindows.c")
elseif(CMAKE_SYSTEM_NAME STREQUAL "Darwin")
    list(APPEND GraphicsSources
        "Private/Window/FWindowMacOS.c")
elseif(CMAKE_SYSTEM_NAME STREQUAL "Android")
    list(APPEND GraphicsSources
        "Private/Window/FWindowAndroid.c")
elseif(CMAKE_SYSTEM_NAME STREQUAL "Linux")
    list(APPEND GraphicsSources
        "Private/Window/FWindowXLib.c")
endif()

list(APPEND GraphicsSources
    "Private/enMeshParser.c")

# Vulkan API
if (Vulkan_FOUND)
    list(APPEND GraphicsSources
        "Private/Vulkan/FVulkanExtensions.c"    
        "Private/Vulkan/FVulkanFormats.c"
        "Private/Vulkan/FVulkanMemoryAllocator.cpp" 
        "Private/Vulkan/FWindowVulkan.c"
        "Private/Vulkan/FGraphicsVulkan.c"
        "Private/Vulkan/FImageVulkan.c"
        "Private/Vulkan/FPipelineVulkan.c")
endif()

add_library(EnterpriseGraphics STATIC ${GraphicsSources})
target_include_directories(EnterpriseGraphics PUBLIC "Public")
target_link_libraries(EnterpriseGraphics EnterpriseCore EnterpriseResource)

# Include ThirdParty Libraries
if(CMAKE_SYSTEM_NAME STREQUAL "Darwin")
    list(APPEND GraphicsSources
        "Private/Window/FWindowMacOS.c")
elseif(CMAKE_SYSTEM_NAME STREQUAL "Android")
    list(APPEND GraphicsSources
        "Private/Window/FWindowAndroid.c")
elseif(CMAKE_SYSTEM_NAME STREQUAL "Linux")
    target_link_libraries(EnterpriseGraphics xcb)
endif()

if (Vulkan_FOUND)
    target_include_directories(EnterpriseGraphics PUBLIC ${Vulkan_INCLUDE_DIR})
    target_link_libraries(EnterpriseGraphics ${Vulkan_LIBRARY})
    target_compile_definitions(EnterpriseGraphics PUBLIC ENTERPRISE_GRAPHICS_VULKAN)

    target_link_libraries(EnterpriseGraphics VulkanMemoryAllocator)
endif()

if (ENTERPRISE_GRAPHICS_X11)
    target_compile_definitions(EnterpriseGraphics PUBLIC ENTERPRISE_GRAPHICS_X11)
elseif (ENTERPRISE_GRAPHICS_WAYLAND)
    target_compile_definitions(EnterpriseGraphics PUBLIC ENTERPRISE_GRAPHICS_WAYLAND)
endif()