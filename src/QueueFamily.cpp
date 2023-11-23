#include "QueueFamily.hpp"

#include <set>

using Indices = QueueFamily::Indices;

bool Indices::isComplete() const {
  return graphicsFamily.has_value() && presentFamily.has_value() &&
         transferFamily.has_value();
}

Indices::QueueCreateInfos QueueFamily::Indices::getQueueCreateInfos() const {
  std::set<uint32_t> uniqueFamilies = {
      graphicsFamily.value(),
      presentFamily.value(),
      transferFamily.has_value()};

  float queuePriority = 1.0f;
  QueueCreateInfos queueCreateInfos;

  for (uint32_t queueFamilyIndex : uniqueFamilies) {
    const auto queueCreateInfo =
        vk::DeviceQueueCreateInfo()
            .setQueueFamilyIndex(queueFamilyIndex)
            .setQueueCount(1)
            .setPQueuePriorities(&queuePriority);

    queueCreateInfos.emplace_back(queueCreateInfo);
  }

  return queueCreateInfos;
}

Indices QueueFamily::findIndices(
    const vk::PhysicalDevice& device, const vk::SurfaceKHR& surface
) {
  Indices indices;

  const auto families = device.getQueueFamilyProperties();

  for (uint32_t familyIndex = 0; familyIndex < families.size(); ++familyIndex) {
    const auto isGraphicsFamily =
        families[familyIndex].queueFlags & vk::QueueFlagBits::eGraphics;

    const auto isTransferFamily =
        families[familyIndex].queueFlags & vk::QueueFlagBits::eTransfer;

    if (isGraphicsFamily) {
      indices.graphicsFamily = familyIndex;
    }

    if (isTransferFamily && !isGraphicsFamily) {
      indices.transferFamily = familyIndex;
    }

    if (device.getSurfaceSupportKHR(familyIndex, surface)) {
      indices.presentFamily = familyIndex;
    }

    if (indices.isComplete()) {
      break;
    }
  }

  return indices;
}
