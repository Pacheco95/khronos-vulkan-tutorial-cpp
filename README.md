# C++ Khronos Tutorial for Linux

Reimplementation of the original
[C tutorial](https://docs.vulkan.org/tutorial/latest/00_Introduction.html)
using the [Vulkan-Hpp](https://github.com/KhronosGroup/Vulkan-Hpp) binding.

General information [here](https://github.com/Pacheco95/khronos-vulkan-tutorial-cpp/blob/main/README.md).

# Adjustments

- Renamed class `HelloTriangleApplication` to `Application` since in the future this code will render much more
  complex geometry than triangles.

- Moved code related to GLFW window management to a separate class.
  I did this because it is very likely you will want to change the window management library in the future,
  e.g. [Dear ImGUI](https://github.com/ocornut/imgui).
  Thus, when you change window management library it will be a lot easier since there will be less code to change,
  and it will be organized.

- All classes have separated definition and implementation. Benefits of doing this:
    - The code is encapsulated. Implementation details are separate from the API interface.
      If in the future you decide to extract parts of the application to a library it will be much easier to do.
    - Compilation time is reduced.
    - Visibility of dependency usages. When you put everything on a single class, there will be lots of `#include`
      macros, and you will likely get lost on who depends on who.

# Navigation

[🌐 Original tutorial](https://docs.vulkan.org/tutorial/latest/03_Drawing_a_triangle/00_Setup/00_Base_code.html)

[⏮ Development environment](https://github.com/Pacheco95/khronos-vulkan-tutorial-cpp/tree/linux/01-development-environment)

[⏭ Next step](https://github.com/Pacheco95/khronos-vulkan-tutorial-cpp/tree/linux/02-drawing-triangle/01-setup/02-instance)

[🔄 Diff from previous step](https://github.com/Pacheco95/khronos-vulkan-tutorial-cpp/compare/linux/01-development-environment...linux/02-drawing-triangle/01-setup/01-base-code)
