# Linux ❘ Drawing ❘ Rendering and presentation

Summary [here](https://github.com/Pacheco95/khronos-vulkan-tutorial-cpp/tree/linux-summary).

# Adjustments

- Created `NO_TIMEOUT` constant to represent disabled timeouts and avoid magic numbers.
- Used the enhanced `Vulkan-Hpp` functions to automatically handle error results.
  You can see that our version of `Application::createSyncObjects()` is way simpler than the original.
  This works for us because we didn't set defined the `VULKAN_HPP_DISABLE_ENHANCED_MODE` macro.
  If we did this, we would have to check the results by ourselves.

# Navigation

[🌐 Original tutorial](
https://docs.vulkan.org/tutorial/latest/03_Drawing_a_triangle/03_Drawing/02_Rendering_and_presentation.html)

[⏮ Drawing a triangle / Drawing / Command buffers](
https://github.com/Pacheco95/khronos-vulkan-tutorial-cpp/tree/linux/02-drawing-triangle/04-drawing/02-command-buffers)

[⏭ Drawing a triangle / Drawing / Frames in flight](
https://github.com/Pacheco95/khronos-vulkan-tutorial-cpp/tree/linux/02-drawing-triangle/04-drawing/04-frames-in-flight)

[🔄 Diff from previous step](
https://github.com/Pacheco95/khronos-vulkan-tutorial-cpp/compare/linux/02-drawing-triangle/04-drawing/02-command-buffers...linux/02-drawing-triangle/04-drawing/03-rendering-and-presentation)
