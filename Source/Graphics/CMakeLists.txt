
# Find the third party libs
find_package(Vulkan REQUIRED)

set(GraphicsSources "")

# Vulkan API
if (Vulkan_FOUND)
    list(APPEND GraphicsSources
        "Private/Context/Vulkan/FVulkanExtensions.c"    
        "Private/Context/Vulkan/FVulkanFormats.c"
        "Private/Context/Vulkan/FVulkanMemoryAllocator.cpp" 
        "Private/Context/Vulkan/FGraphicsVulkan.c"
        "Private/Context/Vulkan/FRenderPassVulkan.c"
        "Private/Context/Vulkan/FImageVulkan.c"
        "Private/Context/Vulkan/FFramebufferVulkan.c")
endif()



add_library(EnterpriseGraphics STATIC ${GraphicsSources})
target_include_directories(EnterpriseGraphics PUBLIC "Public")
target_link_libraries(EnterpriseGraphics EnterpriseCore EnterpriseWindow)

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