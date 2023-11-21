#pragma once

#include <cstdint>
#include <vulkan/vulkan.hpp>

struct Config {
  static constexpr uint16_t WINDOW_WIDTH = 800;

  static constexpr uint16_t WINDOW_HEIGHT = 600;

  static constexpr char WINDOW_TITLE[] = "Vulkan";

  static constexpr char APP_NAME[] = "Vulkan Engine";

  static constexpr std::array VALIDATION_LAYERS = {
      "VK_LAYER_KHRONOS_validation"};

  static const bool IS_VALIDATION_LAYERS_ENABLED;

  static constexpr auto DEBUG_MESSENGER_SEVERITY =
      vk::DebugUtilsMessageSeverityFlagBitsEXT::eWarning |
      vk::DebugUtilsMessageSeverityFlagBitsEXT::eError;

  static constexpr auto DEBUG_MESSAGE_TYPE =
      vk::DebugUtilsMessageTypeFlagBitsEXT::eGeneral |
      vk::DebugUtilsMessageTypeFlagBitsEXT::ePerformance |
      vk::DebugUtilsMessageTypeFlagBitsEXT::eValidation;

  static const std::vector<const char*> DEVICE_EXTENSIONS;

  static constexpr uint8_t MAX_FRAMES_IN_FLIGHT = 2;
};
