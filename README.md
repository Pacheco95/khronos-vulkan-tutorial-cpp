# Khronos Tutorial for C++

Reimplementation of the original
[C tutorial](https://docs.vulkan.org/tutorial/latest/00_Introduction.html)
using the [Vulkan-Hpp](https://github.com/KhronosGroup/Vulkan-Hpp) binding.

For the sake of simplicity and compatibility with the original version, this
tutorial will not use unique handles nor RAII handles.
All resources will be created/released by hand.

**Each [chapter](https://github.com/Pacheco95/khronos-vulkan-tutorial-cpp#chapters) will be developed in a separated
branch**.
Thus, it will be easier to find a specific change from one chapter to another, and we'll not pollute the code with a
bunch of macros.
I also separated branches for each platform.
The tutorial guide for linux wil be in a branch named `linux/...`.
**Those branches will never be merged into `main` due to platform differences**.

> [!IMPORTANT]
> Use the main branch only as a guidance.
> The actual implementations will be in separated branches for each platform for each chapter.

I did the entire tutorial on linux, thus I'll only implement for the linux branch, but you probably will be able to
build for other platforms without much effort.
Feel free to send PRs for other platforms.

Probably I'll make some adjustments on the original code to keep the code readable.
Putting everything in a single class is not a good practice, and you will probably find it difficult to see the
relationship between Vulkan objects this way.
All adjustments will be pointed on its section in the README.md file.

### Implementations

- [Linux](https://github.com/Pacheco95/khronos-vulkan-tutorial-cpp/tree/linux-summary)
