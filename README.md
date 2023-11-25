# Linux ❘ Texture mapping ❘ Loading models

Summary [here](https://github.com/Pacheco95/khronos-vulkan-tutorial-cpp/tree/linux-summary).

# Adjustments

> [!IMPORTANT]
> Fixed a bug preventing the application from picking the correct physical device.
> The bug was introduced
> here:
> https://github.com/Pacheco95/khronos-vulkan-tutorial-cpp/commit/d7ddbfad363e384a8925b0cb4efb5b84297acbcf#r133490971

- Moved mesh loading logic to `ModelLoader` class.
- Added missing class member variables `m_` prefix.
- Added compiler optimization flags to `CMakeLists.txt`.

# Navigation

[🌐 Original tutorial](
https://docs.vulkan.org/tutorial/latest/08_Loading_models.html)

[⏮ Depth buffering](
https://github.com/Pacheco95/khronos-vulkan-tutorial-cpp/tree/linux/06-depth-buffering)

[⏭ Generating Mipmaps](
https://github.com/Pacheco95/khronos-vulkan-tutorial-cpp/tree/linux/08-generating-mipmaps)

[🔄 Diff from previous step](
https://github.com/Pacheco95/khronos-vulkan-tutorial-cpp/compare/linux/06-depth-buffering...linux/07-loading-models)
