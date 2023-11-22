# Linux ❘ Drawing ❘ Swap chain recreation

Summary [here](https://github.com/Pacheco95/khronos-vulkan-tutorial-cpp/tree/linux-summary).

# Adjustments

We did a couple changes in this step:

- Moved all new GLFW calls to our `Window` class and passed a lambda function to `Window::create()` as a callback
  handler for resizes.
- Used `glfwGetWindowAttrib(m_window, GLFW_ICONIFIED) == GLFW_TRUE` instead of calling `glfwGetFramebufferSize()` to
  check if the window is minimized.
- Skip the `renderFrame()` function call instead of "pausing" the game loop.
  I did this because you probably will extend the code in the future to do other things in the main loop that should
  happen even if the window is minimized.
- Again, we used `vk::resultCheck(res, errorMessage)` to handle errors instead of manually checking the error enum and
  throwing exceptions.

# Navigation

[🌐 Original tutorial](
https://docs.vulkan.org/tutorial/latest/03_Drawing_a_triangle/04_Swap_chain_recreation.html)

[⏮ Drawing a triangle / Drawing / Frames in flight](
https://github.com/Pacheco95/khronos-vulkan-tutorial-cpp/tree/linux/02-drawing-triangle/04-drawing/04-frames-in-flight)

[⏭ Vertex buffers / Vertex input description](
https://github.com/Pacheco95/khronos-vulkan-tutorial-cpp/tree/linux/03-vertex-buffers/01-vertex-input-description)

[🔄 Diff from previous step](
https://github.com/Pacheco95/khronos-vulkan-tutorial-cpp/compare/linux/02-drawing-triangle/04-drawing/04-frames-in-flight...linux/02-drawing-triangle/05-swapchain-recreation)
