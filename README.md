# Linux ❘ Graphics pipeline basics ❘ Shader modules

Summary [here](https://github.com/Pacheco95/khronos-vulkan-tutorial-cpp/tree/linux-summary).

# Adjustments

- Moved all shader compilation related code to a separate [CMakeLists.txt](res/shaders/CMakeLists.txt).
  This configuration compiles the shaders automatically only if not already compiled or the source code was changed.
  It will generate the respective `.spv` bytecodes in the same **source directory**.
  Since these `.spv` files can be generated at anytime, we will ignore them at `.gitignore`.
  > [!IMPORTANT]
  > Be sure to set the working directory to the project root directory to correctly load the shaders
- Created a `BinaryLoader` class to read our compiled `.spv` shader bytecodes.
- Added `[spdlog](https://github.com/gabime/spdlog)` as a dependency to help us log useful information during
  development phase.
  `spdlog` also brings `[fmt](https://github.com/fmtlib/fmt)` as a transitive dependency
  which is very handy for handling string interpolation.
- Also added some code at `main.cpp` to configure the logger based on `SPDLOG_ACTIVE_LEVEL` compiler definition.
  `SPDLOG_ACTIVE_LEVEL` is set to `warning` in release builds.

# Navigation

[🌐 Original tutorial](
https://docs.vulkan.org/tutorial/latest/03_Drawing_a_triangle/02_Graphics_pipeline_basics/01_Shader_modules.html)

[⏮ Drawing a triangle / Presentation / Image views](
https://github.com/Pacheco95/khronos-vulkan-tutorial-cpp/tree/linux/02-drawing-triangle/02-presentation/03-image-views)

[⏭ Drawing a triangle / Graphics pipeline basics / Fixed functions](
https://github.com/Pacheco95/khronos-vulkan-tutorial-cpp/tree/linux/02-drawing-triangle/03-graphics-pipeline-basics/03-fixed-functions)

[🔄 Diff from previous step](
https://github.com/Pacheco95/khronos-vulkan-tutorial-cpp/compare/linux/02-drawing-triangle/02-presentation/03-image-views...linux/02-drawing-triangle/03-graphics-pipeline-basics/02-shader-modules)
