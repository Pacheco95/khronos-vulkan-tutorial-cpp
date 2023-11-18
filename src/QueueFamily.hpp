#pragma once

#include <optional>
#include <set>
#include <vulkan/vulkan.hpp>

struct QueueFamily {
  struct Indices {
    typedef std::vector<vk::DeviceQueueCreateInfo> QueueCreateInfos;

    std::optional<uint32_t> graphicsFamily;
    std::optional<uint32_t> presentFamily;

    [[nodiscard]] bool isComplete() const;

    [[nodiscard]] QueueCreateInfos getQueueCreateInfos() const;
  };

  static Indices findIndices(
      const vk::PhysicalDevice& device, const vk::SurfaceKHR& surface
  );
};
