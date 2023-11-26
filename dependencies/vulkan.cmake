include(FetchContent)

add_compile_definitions(GLFW_VULKAN_STATIC)

find_package(Vulkan REQUIRED)
target_include_directories(${EXECUTABLE_NAME}
        PRIVATE
        $ENV{VULKAN_SDK}/include
)
