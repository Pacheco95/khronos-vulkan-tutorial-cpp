# C++ Khronos Tutorial for Linux

Reimplementation for Linux of the original
[C tutorial](https://docs.vulkan.org/tutorial/latest/00_Introduction.html)
using the [Vulkan-Hpp](https://github.com/KhronosGroup/Vulkan-Hpp) binding.

This branch contains all the commits.
Everytime a new chapter is implemented, it is merged here.
You can check the implementation until a given step using the table of contents below.
Each branch contains navigation links for previous/next steps and also a useful link to check the git diff between last
and current step.

> [!IMPORTANT]
> If a bug was found during the development of the current branch, the fix **will not be applied to previous branches**.
> Instead, the next branch that fixes the bug will have a bugfix notice in the table of content below.

* [Development environment](https://github.com/Pacheco95/khronos-vulkan-tutorial-cpp/tree/linux/01-development-environment)
* Setup
    * [Base code](https://github.com/Pacheco95/khronos-vulkan-tutorial-cpp/tree/linux/02-drawing-triangle/01-setup/01-base-code)
    * [Instance](https://github.com/Pacheco95/khronos-vulkan-tutorial-cpp/tree/linux/02-drawing-triangle/01-setup/02-instance)
    * [Validation layers](https://github.com/Pacheco95/khronos-vulkan-tutorial-cpp/tree/linux/02-drawing-triangle/01-setup/03-validation-layers)
    * [Physical devices and queue families](https://github.com/Pacheco95/khronos-vulkan-tutorial-cpp/tree/linux/02-drawing-triangle/01-setup/04-physical-devices-and-queue-families)
    * [Logical device and queues](https://github.com/Pacheco95/khronos-vulkan-tutorial-cpp/tree/linux/02-drawing-triangle/01-setup/05-logical-device-and-queues)
* Presentation
    * [Window surface](https://github.com/Pacheco95/khronos-vulkan-tutorial-cpp/tree/linux/02-drawing-triangle/02-presentation/01-window-surface)
    * [Swap chain](https://github.com/Pacheco95/khronos-vulkan-tutorial-cpp/tree/linux/02-drawing-triangle/02-presentation/02-swapchain)
    * [Image views](https://github.com/Pacheco95/khronos-vulkan-tutorial-cpp/tree/linux/02-drawing-triangle/02-presentation/03-image-views)
* Graphics pipeline basics
    * [Shader modules](https://github.com/Pacheco95/khronos-vulkan-tutorial-cpp/tree/linux/02-drawing-triangle/03-graphics-pipeline-basics/02-shader-modules)
    * [Fixed functions](https://github.com/Pacheco95/khronos-vulkan-tutorial-cpp/tree/linux/02-drawing-triangle/03-graphics-pipeline-basics/03-fixed-functions)
    * [Render passes](https://github.com/Pacheco95/khronos-vulkan-tutorial-cpp/tree/linux/02-drawing-triangle/03-graphics-pipeline-basics/04-render-passes)
