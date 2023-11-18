# Linux ❘ Setup ❘ Validation layers

Summary [here](https://github.com/Pacheco95/khronos-vulkan-tutorial-cpp/tree/linux-summary).

# Adjustments

- Moved queue family related code to `QueueFamily` class.
- Used STL `std::find_if` to find a suitable GPU:
  https://github.com/Pacheco95/khronos-vulkan-tutorial-cpp/blob/b70ae0e1b4f93d075de42c9c391de5afdfb6cf2a/src/Application.cpp#L95

# Navigation

[🌐 Original tutorial](https://docs.vulkan.org/tutorial/latest/03_Drawing_a_triangle/00_Setup/03_Physical_devices_and_queue_families.html)

[⏮ Drawing a triangle / Setup / Validation layers](https://github.com/Pacheco95/khronos-vulkan-tutorial-cpp/tree/linux/02-drawing-triangle/01-setup/03-validation-layers)

[⏭ Drawing a triangle / Setup / Logical device and queues](https://github.com/Pacheco95/khronos-vulkan-tutorial-cpp/tree/linux/02-drawing-triangle/01-setup/05-logical-device-and-queues)

[🔄 Diff from previous step](https://github.com/Pacheco95/khronos-vulkan-tutorial-cpp/compare/linux/02-drawing-triangle/01-setup/03-validation-layers...linux/02-drawing-triangle/01-setup/04-physical-devices-and-queue-families)
