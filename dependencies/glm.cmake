add_compile_definitions(
        GLM_FORCE_DEPTH_ZERO_TO_ONE
        GLM_FORCE_RADIANS
        GLM_ENABLE_EXPERIMENTAL
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

target_include_directories(${EXECUTABLE_NAME} PRIVATE ${glm_SOURCE_DIR})
