# Linux ❘ Presentation ❘ Image Views

Summary [here](https://github.com/Pacheco95/khronos-vulkan-tutorial-cpp/tree/linux-summary).

# Adjustments

- `ImageViewCreateInfo::setComponents()` already set all RGBA members to `vk::ComponentSwizzle::eIdentity`, so, there's
  no need to be redundant here.
- Changed `m_swapChainImageViews.resize()` to `m_swapChainImageViews.reserve()` at `createImageViews()` since `resize()`
  instantiates new elements.
  Thus, we need to use `vector::push_back` or `vector::emplace_back` instead of indexed assignment.
  See https://cplusplus.com/reference/vector/vector/resize/
- Reordered `Application::getSuitableImageCount()` to match `Application.hpp` declaration order.

# Navigation

[🌐 Original tutorial](
https://docs.vulkan.org/tutorial/latest/03_Drawing_a_triangle/01_Presentation/02_Image_views.html)

[⏮ Drawing a triangle / Presentation / Swap chain](
https://github.com/Pacheco95/khronos-vulkan-tutorial-cpp/tree/linux/02-drawing-triangle/02-presentation/02-swapchain)

[⏭ Drawing a triangle / Graphics pipeline basics / Shader modules](
https://github.com/Pacheco95/khronos-vulkan-tutorial-cpp/tree/linux/02-drawing-triangle/03-graphics-pipeline-basics/02-shader-modules)

[🔄 Diff from previous step](
https://github.com/Pacheco95/khronos-vulkan-tutorial-cpp/compare/linux/02-drawing-triangle/02-presentation/02-swapchain...linux/02-drawing-triangle/02-presentation/03-image-views)
