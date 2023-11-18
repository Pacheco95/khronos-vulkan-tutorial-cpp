#pragma once

#include <optional>
#include <vulkan/vulkan.hpp>

struct QueueFamily {
  struct Indices {
    std::optional<uint32_t> graphicsFamily;

    [[nodiscard]] bool isComplete() const;
  };

  static Indices findIndices(const vk::PhysicalDevice& device);
};
