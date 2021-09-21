
# Find the third party libs
find_package(Vulkan REQUIRED)

set(GraphicsSources "")

list(APPEND GraphicsSources
    "Private/FRenderContext.c"
    "Private/FRenderPass.c"
    "Private/FImage.c"
    "Private/FFramebuffer.c")

# Vulkan API
if (Vulkan_FOUND)
    list(APPEND GraphicsSources
        "Private/Context/Vulkan/FVulkanExtensions.c"
        "Private/Context/Vulkan/FVulkanMemoryAllocator.cpp" 
        "Private/Context/Vulkan/FGraphicsContextVulkan.c"
        "Private/Context/Vulkan/FRenderPassVulkan.c"
        "Private/Context/Vulkan/FImageVulkan.c"
        "Private/Context/Vulkan/FFramebufferVulkan.c")
endif()

# Build a specific window file
if (CMAKE_SYSTEM_NAME STREQUAL "Windows")
    list(APPEND GraphicsSources
        "Private/Windows/FWindowWindows.c")
elseif(CMAKE_SYSTEM_NAME STREQUAL "Darwin")
    list(APPEND GraphicsSources
        "Private/Windows/FWindowMacOS.c")
elseif(CMAKE_SYSTEM_NAME STREQUAL "Android")
    list(APPEND GraphicsSources
        "Private/Windows/FWindowAndroid.c")
elseif(CMAKE_SYSTEM_NAME STREQUAL "Linux")
    list(APPEND GraphicsSources
        "Private/Windows/FWindowLinuxX11.c")
endif()


add_library(EnterpriseGraphics SHARED ${GraphicsSources})
target_include_directories(EnterpriseGraphics PUBLIC "Public")
target_link_libraries(EnterpriseGraphics EnterpriseCore)
target_link_libraries(EnterpriseGraphics SDL2)

# Include ThirdParty Libraries
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