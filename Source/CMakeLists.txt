
# Find the required third party libraries
find_package(Vulkan REQUIRED)


set (EnterpriseSources "")


# Build each module
add_subdirectory(Core)
add_subdirectory(Resource)
add_subdirectory(Networking)
add_subdirectory(Window)
add_subdirectory(Graphics)
add_subdirectory(GraphicsUI)
add_subdirectory(Application)