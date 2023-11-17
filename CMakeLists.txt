cmake_minimum_required(VERSION 3.22)
project(KhronosVulkanTutorialHpp CXX)

set(CMAKE_CXX_STANDARD 17)
set(VULKAN_SDK_DIR "$ENV{VULKAN_SDK}")
set(SOURCES src/main.cpp)

add_compile_definitions(GLFW_VULKAN_STATIC)

add_executable(KhronosVulkanTutorialHpp ${SOURCES})

############################ External dependencies #############################
include(FetchContent)

#################################### Vulkan ####################################
find_package(Vulkan REQUIRED)
target_include_directories(KhronosVulkanTutorialHpp
        PRIVATE
        $ENV{VULKAN_SDK}/include
)

##################################### GLFW #####################################
option(GLFW_BUILD_EXAMPLES "Build the GLFW example programs" OFF)
option(GLFW_BUILD_TESTS "Build the GLFW test programs" OFF)
option(GLFW_BUILD_DOCS "Build the GLFW documentation" OFF)
option(GLFW_INSTALL "Generate installation target" OFF)

FetchContent_Declare(
        GLFW
        GIT_REPOSITORY https://github.com/glfw/glfw.git
        GIT_TAG 3.3.8
)

FetchContent_GetProperties(GLFW)
if (NOT GLFW_POPULATED)
  FetchContent_Populate(GLFW)
  add_subdirectory(${glfw_SOURCE_DIR} ${glfw_BINARY_DIR})
endif ()

##################################### GLM ######################################
FetchContent_Declare(
        GLM
        GIT_REPOSITORY https://github.com/g-truc/glm.git
        GIT_TAG 0.9.9.8
)

FetchContent_GetProperties(GLM)
if (NOT GLM_POPULATED)
  FetchContent_Populate(GLM)
  add_subdirectory(${glm_SOURCE_DIR} ${glm_BINARY_DIR})
endif ()

target_include_directories(KhronosVulkanTutorialHpp PRIVATE ${glm_SOURCE_DIR})

target_link_libraries(KhronosVulkanTutorialHpp
        PRIVATE
        Vulkan::Vulkan
        glfw
        dl
        pthread
        X11
        Xxf86vm
        Xrandr
        Xi
)

# Install targets
install(TARGETS KhronosVulkanTutorialHpp DESTINATION bin)
