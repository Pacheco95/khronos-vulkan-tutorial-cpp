# Linux ❘ Loading models

Summary [here](https://github.com/Pacheco95/khronos-vulkan-tutorial-cpp/tree/linux-summary).

# Adjustments

> [!IMPORTANT]
> Fixed a bug preventing the application from picking the correct physical device.
> The bug was introduced
> here:
> https://github.com/Pacheco95/khronos-vulkan-tutorial-cpp/commit/d7ddbfad363e384a8925b0cb4efb5b84297acbcf#r133490971
>
> Fixed another early bug forcing the application to crash after window resizes.
> See https://github.com/Pacheco95/khronos-vulkan-tutorial-cpp/commit/6df9f55ab4346b9347ecbab2570be889f0d8eee2#diff-012cc4ba952523f9f987b2af14b664e01767b27560ee21dc711cc63ce31b3849R122
> and this comment https://github.com/Pacheco95/khronos-vulkan-tutorial-cpp/commit/6df9f55ab4346b9347ecbab2570be889f0d8eee2#r133495542

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
