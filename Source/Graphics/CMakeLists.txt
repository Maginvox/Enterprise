
# Find the third party libs
find_package(Vulkan REQUIRED)


set(GraphicsSources "")

list(APPEND GraphicsSources
    "FRenderPass.c"
    "FImage.c"
    "FFramebuffer.c")

# Vulkan API
if (Vulkan_FOUND)
    list(APPEND GraphicsSources
        "Context/Vulkan/FRenderContextVulkan.c"
        "Context/Vulkan/FRenderPassVulkan.c")
endif()


add_library(EnterpriseGraphics SHARED ${GraphicsSources})
target_include_directories(EnterpriseGraphics PUBLIC "${PROJECT_SOURCE_DIR}/Include/")
target_link_libraries(EnterpriseGraphics EnterpriseCore)


# Include ThirdParty Libraries
if (Vulkan_FOUND)
    target_include_directories(EnterpriseGraphics PUBLIC ${Vulkan_INCLUDE_DIR})

    target_link_libraries(EnterpriseGraphics VulkanMemoryAllocator)
endif()