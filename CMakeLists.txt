cmake_minimum_required(VERSION 3.22)
project(KhronosVulkanTutorialHpp CXX)

set(CMAKE_CXX_STANDARD 17)
set(EXECUTABLE_NAME ${CMAKE_PROJECT_NAME})
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
        src/ModelLoader.cpp
        src/ModelLoader.hpp
        src/SingleTimeCommand.cpp
        src/SingleTimeCommand.hpp
        src/MipmapGenerator.cpp
        src/MipmapGenerator.hpp
)

if (CMAKE_BUILD_TYPE STREQUAL "Release")
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O2")
else ()
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O0")
endif ()

add_executable(${EXECUTABLE_NAME} ${SOURCES})

target_compile_options(${EXECUTABLE_NAME} PRIVATE
        $<$<CXX_COMPILER_ID:MSVC>:/W4 /WX>
        $<$<NOT:$<CXX_COMPILER_ID:MSVC>>:-Wall -Wextra -Wpedantic>
)

############################ External dependencies #############################

include(dependencies/vulkan.cmake)
include(dependencies/glfw.cmake)
include(dependencies/glm.cmake)
include(dependencies/spdlog.cmake)
include(dependencies/stb.cmake)
include(dependencies/tinyobjloader.cmake)

target_link_libraries(${EXECUTABLE_NAME}
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

add_dependencies(${EXECUTABLE_NAME} Shaders)

install(TARGETS ${EXECUTABLE_NAME} DESTINATION bin)
