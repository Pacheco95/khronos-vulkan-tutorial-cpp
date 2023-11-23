#pragma once

#include <optional>
#include <unordered_map>
#include <vulkan/vulkan.hpp>

#include "HashUtils.hpp"

class QueueFamily {
 public:
  struct Indices {
    typedef std::vector<vk::DeviceQueueCreateInfo> QueueCreateInfos;

    std::optional<uint32_t> graphicsFamily;
    std::optional<uint32_t> presentFamily;
    std::optional<uint32_t> transferFamily;

    [[nodiscard]] bool isComplete() const;
    [[nodiscard]] QueueCreateInfos getQueueCreateInfos() const;
  };

  typedef std::tuple<VkPhysicalDevice, VkSurfaceKHR> LookupKey;
  typedef HashUtils::hash<LookupKey> LookupHash;
  typedef std::unordered_map<LookupKey, Indices, LookupHash> LookupTable;

  static Indices findIndices(
      const vk::PhysicalDevice& device, const vk::SurfaceKHR& surface
  );

 private:
  static LookupTable m_indicesCache;

  static Indices findIndicesUncached(
      const vk::PhysicalDevice& device, const vk::SurfaceKHR& surface
  );
};
