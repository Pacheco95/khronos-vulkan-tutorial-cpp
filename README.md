# C++ Khronos Tutorial for Linux

Reimplementation of the original
[C tutorial](https://docs.vulkan.org/tutorial/latest/00_Introduction.html)
using the [Vulkan-Hpp](https://github.com/KhronosGroup/Vulkan-Hpp) binding.

General information [here](https://github.com/Pacheco95/khronos-vulkan-tutorial-cpp/blob/main/README.md).

# Adjustments

- All validation layer related code was moved to `ValidationLayer` class.
- `Config::IS_VALIDATION_LAYERS_ENABLED` was declared as `const` instead of `constexpr` and defined in `Config.cpp`
  instead of `Config.hpp` to avoid compiler warnings saying that expression like this can be (erroneously) simplified:
  ```c++
  if (Config::IS_VALIDATION_LAYERS_ENABLED) {
    // The compiler is warning because Config::IS_VALIDATION_LAYERS_ENABLED is a constexpr
    // and will always evaluate to true or false depending on your build target (Debug / Release)
  }
  ```
- `vkDestroyDebugUtilsMessengerEXT` is now loaded right after the `vk::Instance` creation instead of in `cleanup()`

> [!TIP]
> At this point, you can clearly see the effect of our organization.
> The original formatted code has 231 lines and our `Application.cpp` only has 82. This is ≈ 64% less code!

# Navigation

[🌐 Original tutorial](https://docs.vulkan.org/tutorial/latest/03_Drawing_a_triangle/00_Setup/02_Validation_layers.html)

[⏮ Drawing a triangle / Setup / Instance](https://github.com/Pacheco95/khronos-vulkan-tutorial-cpp/tree/linux/02-drawing-triangle/01-setup/02-instance)

[⏭ Drawing a triangle / Setup / Physical devices and queue families](https://github.com/Pacheco95/khronos-vulkan-tutorial-cpp/tree/linux/02-drawing-triangle/01-setup/04-physical-devices-and-queue-families)

[🔄 Diff from previous step](https://github.com/Pacheco95/khronos-vulkan-tutorial-cpp/compare/linux/02-drawing-triangle/01-setup/02-instance...linux/02-drawing-triangle/01-setup/03-validation-layers)
