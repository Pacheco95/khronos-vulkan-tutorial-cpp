cmake_minimum_required(VERSION 3.22)
project(KhronosVulkanTutorialHpp CXX)

set(CMAKE_CXX_STANDARD 17)
set(VULKAN_SDK_DIR "$ENV{VULKAN_SDK}")
set(SOURCES
        src/main.cpp
        src/Application.cpp
        src/Application.hpp
        src/Window.cpp
        src/Window.hpp
        src/Config.cpp
        src/Config.hpp
        src/Utils.cpp
        src/Utils.hpp
        src/ValidationLayer.cpp
        src/ValidationLayer.hpp
        src/QueueFamily.cpp
        src/QueueFamily.hpp
        src/BinaryLoader.cpp
        src/BinaryLoader.hpp
        src/Vertex.cpp
        src/Vertex.hpp
)

add_compile_definitions(GLFW_VULKAN_STATIC)

add_executable(KhronosVulkanTutorialHpp ${SOURCES})

target_compile_options(KhronosVulkanTutorialHpp PRIVATE
        $<$<CXX_COMPILER_ID:MSVC>:/W4 /WX>
        $<$<NOT:$<CXX_COMPILER_ID:MSVC>>:-Wall -Wextra -Wpedantic>
)

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
add_compile_definitions(
        GLM_FORCE_DEPTH_ZERO_TO_ONE
        GLM_FORCE_RADIANS
)

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


################################### SPDLog #####################################
if (CMAKE_BUILD_TYPE STREQUAL "Release")
  add_compile_definitions(SPDLOG_ACTIVE_LEVEL=SPDLOG_LEVEL_WARN)
else ()
  add_compile_definitions(SPDLOG_ACTIVE_LEVEL=SPDLOG_LEVEL_TRACE)
endif ()

FetchContent_Declare(
        spdlog
        GIT_REPOSITORY https://github.com/gabime/spdlog.git
        GIT_TAG v1.12.0
)

FetchContent_GetProperties(spdlog)
if (NOT spdlog_POPULATED)
  FetchContent_Populate(spdlog)
  add_subdirectory(${spdlog_SOURCE_DIR} ${spdlog_BINARY_DIR})
endif ()

############################ End of dependencies ###############################

target_link_libraries(KhronosVulkanTutorialHpp
        PRIVATE
        Vulkan::Vulkan
        glfw
        spdlog
        dl
        pthread
        X11
        Xxf86vm
        Xrandr
        Xi
)

add_subdirectory(res/shaders)

add_dependencies(KhronosVulkanTutorialHpp Shaders)

install(TARGETS KhronosVulkanTutorialHpp DESTINATION bin)
