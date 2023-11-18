#include "QueueFamily.hpp"

using Indices = QueueFamily::Indices;

bool Indices::isComplete() const { return graphicsFamily.has_value(); }

Indices QueueFamily::findIndices(const vk::PhysicalDevice& device) {
  Indices indices;

  const auto& queueFamilies = device.getQueueFamilyProperties();

  for (uint32_t i = 0; i < queueFamilies.size(); i++) {
    if (queueFamilies[i].queueFlags & vk::QueueFlagBits::eGraphics) {
      indices.graphicsFamily = i;
    }

    if (indices.isComplete()) {
      break;
    }
  }

  return indices;
}
