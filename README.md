# Linux ❘ Vertex buffers ❘ Vertex input description

Summary [here](https://github.com/Pacheco95/khronos-vulkan-tutorial-cpp/tree/linux-summary).

# Adjustments

> [!CAUTION]
> All usages of temporary bound to function calls (`const auto& var = func()`) were removed.
> Since I was compiling the code with `CLang 17`, everything was working just fine.
> When I tried to compile with `GCC 11.4.0` the application crashed with segmentation faults.
> Debugging the code, I realized that the reference lifetime was ending right before the assignment and the reference
> became invalid, so I removed all of them and used the copy assignments to prevent this error.
> See
> * https://github.com/KhronosGroup/Vulkan-Hpp/issues/1736
> * https://stackoverflow.com/questions/17362673/temporary-lifetime-extension

- Added extra warning & error compiler flags.
- Fixed compiler warnings.

# Navigation

[🌐 Original tutorial](
https://docs.vulkan.org/tutorial/latest/04_Vertex_buffers/00_Vertex_input_description.html)

[⏮ Drawing a triangle / Swap chain recreation](
https://github.com/Pacheco95/khronos-vulkan-tutorial-cpp/tree/linux/02-drawing-triangle/05-swapchain-recreation)

[⏭ Vertex buffers / Vertex buffer creation](
https://github.com/Pacheco95/khronos-vulkan-tutorial-cpp/tree/linux/03-vertex-buffers/02-vertex-buffer-creation)

[🔄 Diff from previous step](
https://github.com/Pacheco95/khronos-vulkan-tutorial-cpp/compare/linux/02-drawing-triangle/05-swapchain-recreation...linux/03-vertex-buffers/01-vertex-input-description)
