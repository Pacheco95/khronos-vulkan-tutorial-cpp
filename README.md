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
> Instead, the next branch that fixes the bug will have a bugfix notice (🐞) in the table of contents below.

> [!CAUTION]
> A critical bug was found in
> the [Loading models](https://github.com/Pacheco95/khronos-vulkan-tutorial-cpp/tree/linux/07-loading-models) step.
> It was caused in a very early commit, and you should fix it as soon as possible, otherwise the application will
> probably pick the wrong physical device.
> If you don't fix this bug early, then it may hinder your experience through the tutorial until you reach the chapter
> where the bug was fixed.
> This step also fixes another bug forcing application to crash on window resize.

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
    * [Conclusion](https://github.com/Pacheco95/khronos-vulkan-tutorial-cpp/tree/linux/02-drawing-triangle/03-graphics-pipeline-basics/05-conclusion)
* Drawing
    * [Frame buffers](https://github.com/Pacheco95/khronos-vulkan-tutorial-cpp/tree/linux/02-drawing-triangle/04-drawing/01-frame-buffers)
    * [Command buffers](https://github.com/Pacheco95/khronos-vulkan-tutorial-cpp/tree/linux/02-drawing-triangle/04-drawing/02-command-buffers)
    * [Rendering and presentation](https://github.com/Pacheco95/khronos-vulkan-tutorial-cpp/tree/linux/02-drawing-triangle/04-drawing/03-rendering-and-presentation)
    * [Frames in flight](https://github.com/Pacheco95/khronos-vulkan-tutorial-cpp/tree/linux/02-drawing-triangle/04-drawing/04-frames-in-flight)
    * [Swap chain recreation](https://github.com/Pacheco95/khronos-vulkan-tutorial-cpp/tree/linux/02-drawing-triangle/05-swapchain-recreation)
* Vertex buffers
    * [Vertex input description](https://github.com/Pacheco95/khronos-vulkan-tutorial-cpp/tree/linux/03-vertex-buffers/01-vertex-input-description)
      🐞
    * [Vertex buffer creation](https://github.com/Pacheco95/khronos-vulkan-tutorial-cpp/tree/linux/03-vertex-buffers/02-vertex-buffer-creation)
    * [Staging buffer](https://github.com/Pacheco95/khronos-vulkan-tutorial-cpp/tree/linux/03-vertex-buffers/03-staging-buffer)
    * [Index buffer](https://github.com/Pacheco95/khronos-vulkan-tutorial-cpp/tree/linux/03-vertex-buffers/04-index-buffer)
    * [Descriptor layout and buffer](https://github.com/Pacheco95/khronos-vulkan-tutorial-cpp/tree/linux/04-uniform-buffers/01-descriptor-layout-and-buffer)
    * [Descriptor pool and sets](https://github.com/Pacheco95/khronos-vulkan-tutorial-cpp/tree/linux/04-uniform-buffers/02-descriptor-pool-and-sets)
* Texture mapping
    * [Images](https://github.com/Pacheco95/khronos-vulkan-tutorial-cpp/tree/linux/05-texture-mapping/01-images)
    * [Image view and sampler](https://github.com/Pacheco95/khronos-vulkan-tutorial-cpp/tree/linux/05-texture-mapping/02-image-view-and-sampler)
    * [Combined image sampler](https://github.com/Pacheco95/khronos-vulkan-tutorial-cpp/tree/linux/05-texture-mapping/03-combined-image-sampler)
* [Depth buffering](https://github.com/Pacheco95/khronos-vulkan-tutorial-cpp/tree/linux/06-depth-buffering)
* [Loading models](https://github.com/Pacheco95/khronos-vulkan-tutorial-cpp/tree/linux/07-loading-models) 🐞
