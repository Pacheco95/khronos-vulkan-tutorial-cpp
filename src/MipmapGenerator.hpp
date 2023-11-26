#pragma once

#include <vulkan/vulkan.hpp>

#include "SingleTimeCommand.hpp"

class MipmapGenerator {
 public:
  struct MipmapProps {
    vk::Image& image;
    const vk::Format& imageFormat;
    const int32_t texWidth;
    const int32_t texHeight;
    const uint32_t mipLevels;
  };

  static void generate(
      const MipmapProps& mipmapProps,
      SingleTimeCommand& singleTimeCommand,
      const vk::PhysicalDevice& physicalDevice
  );
};
