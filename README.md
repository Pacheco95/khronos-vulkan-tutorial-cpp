# Linux ❘ Presentation ❘ Swap chain

Summary [here](https://github.com/Pacheco95/khronos-vulkan-tutorial-cpp/tree/linux-summary).

# Adjustments

- Moved struct `SwapChainSupportDetails` to `Application` class and added constructor for convenience.
- Improved methods constness and added `[[nodiscard]]` attributes.
- Moved device extensions vector to `Config` class.
- Created `noneOf` template utility function to make code cleaner.
- Removed unused `Window` constructor.
- Extracted extent related code at `chooseSwapExtent()` to `Window::getExtent()`.
- Formatted `QueueFamily::Indices::getQueueCreateInfos()` method.

# Navigation

[🌐 Original tutorial](
https://docs.vulkan.org/tutorial/latest/03_Drawing_a_triangle/01_Presentation/01_Swap_chain.html)

[⏮ Drawing a triangle / Presentation / Window surface](
https://github.com/Pacheco95/khronos-vulkan-tutorial-cpp/tree/linux/02-drawing-triangle/02-presentation/01-window-surface)

[⏭ Drawing a triangle / Presentation / Image views](
https://github.com/Pacheco95/khronos-vulkan-tutorial-cpp/tree/linux/02-drawing-triangle/02-presentation/03-image-views)

[🔄 Diff from previous step](
https://github.com/Pacheco95/khronos-vulkan-tutorial-cpp/compare/linux/02-drawing-triangle/02-presentation/01-window-surface...linux/02-drawing-triangle/02-presentation/02-swapchain)
