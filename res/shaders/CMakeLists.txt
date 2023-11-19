set(SHADERS_SRC_DIR ${CMAKE_CURRENT_SOURCE_DIR})
set(SHADERS_OUT_DIR ${CMAKE_CURRENT_SOURCE_DIR})

file(GLOB_RECURSE SHADER_FILES
        "${SHADERS_SRC_DIR}/*.frag"
        "${SHADERS_SRC_DIR}/*.vert"
)

foreach (GLSL ${SHADER_FILES})
  get_filename_component(FILE_NAME ${GLSL} NAME)
  set(SPIRV "${SHADERS_OUT_DIR}/${FILE_NAME}.spv")
  add_custom_command(
          OUTPUT ${SPIRV}
          COMMAND glslc ${GLSL} -o ${SPIRV}
          DEPENDS ${GLSL})
  list(APPEND SPIRV_BINARY_FILES ${SPIRV})
endforeach (GLSL)

add_custom_target(
        Shaders
        DEPENDS ${SPIRV_BINARY_FILES}
)
