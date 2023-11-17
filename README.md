# C++ Khronos Tutorial for Linux

Reimplementation of the original
[C tutorial](https://docs.vulkan.org/tutorial/latest/00_Introduction.html)
using the [Vulkan-Hpp](https://github.com/KhronosGroup/Vulkan-Hpp) binding.

General information [here](https://github.com/Pacheco95/khronos-vulkan-tutorial-cpp/blob/main/README.md).

# Requirements

- C++17 compiler. I used the `Ubuntu clang version 17.0.5 (++20231114093837+98bfdac5ce82-1~exp1~20231114093938.66)` but
  16 should work fine.
- CMake 3.22 or higher. I used `3.26.4`.
- A build system. I'm using [Ninja](https://ninja-build.org/), but you can use Unix Makefiles if you want.

# Setup

The original tutorial installs dependencies using development library packages.
This is a quick start, but you lose control over your dependency versions.
I decided to manage the dependencies explicitly, but automatically, using CMake.

To set up your development environment, follow these steps:

1. Download and extract the [Vulkan SDK](https://vulkan.lunarg.com). I used version `1.3.268.0`.
2. Follow the SDK [installation steps](https://vulkan.lunarg.com/doc/sdk/1.3.268.0/linux/getting_started.html).
3. `git clone https://github.com/Pacheco95/khronos-vulkan-tutorial-cpp`
4. `cd khronos-vulkan-tutorial-cpp`
5. (Optional) `bun install` or npm, pnpm, yarn, etc.
   This will configure git hooks to automatically format the source code before commits.
6. Build the project:

```shell
# Generate build files
mkdir build && cd build

# This will download all dependencies to build/_deps directory
cmake -DCMAKE_MAKE_PROGRAM=ninja -DCMAKE_CXX_COMPILER=clang++-17 -G "Ninja Multi-Config" -S .. -B .

# Build
cmake --build . --target KhronosVulkanTutorialHpp --config Debug -j 4 # Replace 4 with your CPU core count

# Run
./Debug/KhronosVulkanTutorialHpp
```

If you did set up the SDK correctly, you'll see a message:

```
VULKAN_SDK found at <your install location>
```

# Adjustments

- Moved `GLM_FORCE_RADIANS` and `GLM_FORCE_DEPTH_ZERO_TO_ONE` includes to
  https://github.com/Pacheco95/khronos-vulkan-tutorial-cpp/blob/7b3324002e9371b31a987ff7335b2e80145887ca/CMakeLists.txt#L41

- Deleted `#define GLFW_INCLUDE_VULKAN` since it imports the `vulkan.h` and we are now using `vulkan.hpp`
