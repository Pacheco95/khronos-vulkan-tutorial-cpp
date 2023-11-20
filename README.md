# Linux ❘ Graphics pipeline basics ❘ Render passes

Summary [here](https://github.com/Pacheco95/khronos-vulkan-tutorial-cpp/tree/linux-summary).

# Adjustments

- Used `Vulkan-hpp` helper methods to set count/pointer functions.
  Instead of using
  ```c++
  info.setColorAttachmentCount(count)
      .setPColorAttachment(&attachment)
  ```
  We use
  ```c++
  info.setColorAttachments(attachment)
  ```
  It will automatically cast attachment to a single element container if attachment was not already a container.

# Navigation

[🌐 Original tutorial](
https://docs.vulkan.org/tutorial/latest/03_Drawing_a_triangle/02_Graphics_pipeline_basics/03_Render_passes.html)

[⏮ Drawing a triangle / Graphics pipeline basics / Fixed functions](
https://github.com/Pacheco95/khronos-vulkan-tutorial-cpp/tree/linux/02-drawing-triangle/03-graphics-pipeline-basics/03-fixed-functions)

[⏭ Drawing a triangle / Graphics pipeline basics / Conclusion](
https://github.com/Pacheco95/khronos-vulkan-tutorial-cpp/tree/linux/02-drawing-triangle/03-graphics-pipeline-basics/05-conclusion)

[🔄 Diff from previous step](
https://github.com/Pacheco95/khronos-vulkan-tutorial-cpp/compare/linux/02-drawing-triangle/03-graphics-pipeline-basics/03-fixed-functions...linux/02-drawing-triangle/03-graphics-pipeline-basics/04-render-passes)
