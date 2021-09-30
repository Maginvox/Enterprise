
# Find the required third party libraries
find_package(Vulkan REQUIRED)


set(EnterpriseSources "")
add_compile_definitions(ENTERPRISE_DEBUG)

if (CMAKE_SYSTEM_NAME STREQUAL "Windows")
    add_compile_definitions(ENTERPRISE_WINDOWS)
elseif(CMAKE_SYSTEM_NAME STREQUAL "Darwin")
    add_compile_definitions(ENTERPRISE_MACOS)
elseif(CMAKE_SYSTEM_NAME STREQUAL "Android")
    add_compile_definitions(ENTERPRISE_ANDROID)
elseif(CMAKE_SYSTEM_NAME STREQUAL "Linux")
    add_compile_definitions(ENTERPRISE_LINUX)
endif()

# Build each module
add_subdirectory(Core)
add_subdirectory(Resource)
add_subdirectory(Networking)

add_subdirectory(Graphics)
add_subdirectory(GraphicsUI)
add_subdirectory(Application)