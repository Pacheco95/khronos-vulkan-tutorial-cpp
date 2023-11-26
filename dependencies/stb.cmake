FetchContent_Declare(
        stb
        GIT_REPOSITORY https://github.com/nothings/stb.git
        GIT_TAG master
)

add_compile_definitions(STB_IMAGE_IMPLEMENTATION)

FetchContent_GetProperties(stb)
if (NOT stb_POPULATED)
  FetchContent_Populate(stb)
endif ()

target_include_directories(${EXECUTABLE_NAME} PRIVATE ${stb_SOURCE_DIR})
