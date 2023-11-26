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
