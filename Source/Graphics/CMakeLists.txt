set(GraphicsSources "")

list(APPEND GraphicsSources
    "FRenderPass.c"
    "FImage.c"
    "FFramebuffer.c")

add_library(EnterpriseGraphics SHARED ${GraphicsSources})
target_include_directories(EnterpriseGraphics PUBLIC "${PROJECT_SOURCE_DIR}/Include/")