#pragma once

#include <vulkan/vulkan.hpp>

class ValidationLayer {
 public:
  static bool checkValidationLayerSupport();

  static VKAPI_ATTR VkBool32 VKAPI_CALL debugCallback(
      [[maybe_unused]] VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity,
      [[maybe_unused]] VkDebugUtilsMessageTypeFlagsEXT messageType,
      const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData,
      [[maybe_unused]] void* pUserData
  );

  static void loadDebugUtilsMessengerFunctions(const vk::Instance& instance);

  static vk::DebugUtilsMessengerCreateInfoEXT
  getDebugUtilsMessengerCreateInfoEXT();
};
