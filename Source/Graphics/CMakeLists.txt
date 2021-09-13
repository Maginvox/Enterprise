
# Find the third party libs
find_package(Vulkan REQUIRED)

set(GraphicsSources "")

list(APPEND GraphicsSources
    "FRenderContext.c"
    "FRenderPass.c"
    "FImage.c"
    "FFramebuffer.c")

# Vulkan API
if (Vulkan_FOUND)
    list(APPEND GraphicsSources
        "Context/Vulkan/FVulkanExtensions.c"
        "Context/Vulkan/FVulkanMemoryAllocator.cpp" 
        "Context/Vulkan/FRenderContextVulkan.c"
        "Context/Vulkan/FRenderPassVulkan.c")
endif()


add_library(EnterpriseGraphics SHARED ${GraphicsSources})
target_include_directories(EnterpriseGraphics PUBLIC "${PROJECT_SOURCE_DIR}/Include/")
target_link_libraries(EnterpriseGraphics EnterpriseCore SDL2)

# Include ThirdParty Libraries
if (Vulkan_FOUND)
    target_include_directories(EnterpriseGraphics PUBLIC ${Vulkan_INCLUDE_DIR})
    target_compile_definitions(EnterpriseGraphics PUBLIC ENTERPRISE_GRAPHICS_VULKAN)

    target_link_libraries(EnterpriseGraphics VulkanMemoryAllocator)
endif()