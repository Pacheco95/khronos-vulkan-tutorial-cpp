add_compile_definitions(TINYOBJLOADER_IMPLEMENTATION)

FetchContent_Declare(
        tinyobjloader
        GIT_REPOSITORY https://github.com/tinyobjloader/tinyobjloader.git
        GIT_TAG release
)

FetchContent_GetProperties(tinyobjloader)
if (NOT tinyobjloader_POPULATED)
  FetchContent_Populate(tinyobjloader)
  add_subdirectory(${tinyobjloader_SOURCE_DIR} ${tinyobjloader_BINARY_DIR})
endif ()

target_link_libraries(${EXECUTABLE_NAME} PRIVATE tinyobjloader)
