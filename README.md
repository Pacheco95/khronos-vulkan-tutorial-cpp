# Linux ❘ Graphics pipeline basics ❘ Conclusion

Summary [here](https://github.com/Pacheco95/khronos-vulkan-tutorial-cpp/tree/linux-summary).

# Adjustments

- The `vk::Device::createGraphicsPipeline()` returns a `vk::ResultValue<vk::Pipeline>`, so we can use `std::tie()` to
  extract the result and the pipeline.
  In cases like this, use the `vk::checkResult()` function to validate that the operation succeeded.

  ```c++
    vk::Result createPipelineResult;
    vk::Pipeline pipeline;
    std::tie(createPipelineResult, pipeline) = device.createGraphicsPipeline({}, pipelineInfo);
    vk::resultCheck(createPipelineResult, "Failed to create graphics pipeline");
  ```

# Navigation

[🌐 Original tutorial](
https://docs.vulkan.org/tutorial/latest/03_Drawing_a_triangle/02_Graphics_pipeline_basics/04_Conclusion.html)

[⏮ Drawing a triangle / Graphics pipeline basics / Render passes](
https://github.com/Pacheco95/khronos-vulkan-tutorial-cpp/tree/linux/02-drawing-triangle/03-graphics-pipeline-basics/04-render-passes)

[⏭ Drawing a triangle / Drawing / Frame buffers](
https://github.com/Pacheco95/khronos-vulkan-tutorial-cpp/tree/linux/02-drawing-triangle/04-drawing/01-frame-buffers)

[🔄 Diff from previous step](
https://github.com/Pacheco95/khronos-vulkan-tutorial-cpp/compare/linux/02-drawing-triangle/03-graphics-pipeline-basics/04-render-passes...linux/02-drawing-triangle/03-graphics-pipeline-basics/05-conclusion)
