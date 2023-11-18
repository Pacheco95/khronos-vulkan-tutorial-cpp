#include "ValidationLayer.hpp"

#include <iostream>

/**
 * These functions are not automatically loaded by the Vulkan loader.
 * That's why we created
 * @code
 * ValidationLayer::loadDebugUtilsMessengerFunctions(const vk::Instance&)
 * @endcode
 */
PFN_vkCreateDebugUtilsMessengerEXT pfnVkCreateDebugUtilsMessengerEXT;
PFN_vkDestroyDebugUtilsMessengerEXT pfnVkDestroyDebugUtilsMessengerEXT;

[[maybe_unused]] VKAPI_ATTR VkResult VKAPI_CALL vkCreateDebugUtilsMessengerEXT(
    VkInstance instance,
    const VkDebugUtilsMessengerCreateInfoEXT* pCreateInfo,
    const VkAllocationCallbacks* pAllocator,
    VkDebugUtilsMessengerEXT* pMessenger
) {
  return pfnVkCreateDebugUtilsMessengerEXT(
      instance, pCreateInfo, pAllocator, pMessenger
  );
}

[[maybe_unused]] VKAPI_ATTR void VKAPI_CALL vkDestroyDebugUtilsMessengerEXT(
    VkInstance instance,
    VkDebugUtilsMessengerEXT messenger,
    VkAllocationCallbacks const* pAllocator
) {
  return pfnVkDestroyDebugUtilsMessengerEXT(instance, messenger, pAllocator);
}

bool ValidationLayer::checkValidationLayerSupport() {
  const std::vector<vk::LayerProperties>& availableLayers =
      vk::enumerateInstanceLayerProperties();

  for (const auto& requestedLayerName : Config::VALIDATION_LAYERS) {
    bool layerFound = false;

    for (const auto& layerProperties : availableLayers) {
      if (strcmp(requestedLayerName, layerProperties.layerName) == 0) {
        layerFound = true;
        break;
      }
    }

    if (!layerFound) {
      return false;
    }
  }

  return true;
}

VkBool32 ValidationLayer::debugCallback(
    [[maybe_unused]] VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity,
    [[maybe_unused]] VkDebugUtilsMessageTypeFlagsEXT messageType,
    const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData,
    [[maybe_unused]] void* pUserData
) {
  std::cerr << "Validation layer: " << pCallbackData->pMessage << std::endl;

  return VK_FALSE;
}

void ValidationLayer::loadDebugUtilsMessengerFunctions(
    const vk::Instance& instance
) {
  pfnVkCreateDebugUtilsMessengerEXT =
      reinterpret_cast<PFN_vkCreateDebugUtilsMessengerEXT>(
          instance.getProcAddr("vkCreateDebugUtilsMessengerEXT")
      );

  if (!pfnVkCreateDebugUtilsMessengerEXT) {
    throw std::runtime_error(
        "GetInstanceProcAddr: Unable to find "
        "pfnVkCreateDebugUtilsMessengerEXT function."
    );
  }

  pfnVkDestroyDebugUtilsMessengerEXT =
      reinterpret_cast<PFN_vkDestroyDebugUtilsMessengerEXT>(
          instance.getProcAddr("vkDestroyDebugUtilsMessengerEXT")
      );

  if (!pfnVkDestroyDebugUtilsMessengerEXT) {
    throw std::runtime_error(
        "GetInstanceProcAddr: Unable to find "
        "pfnVkDestroyDebugUtilsMessengerEXT function."
    );
  }
}

vk::DebugUtilsMessengerCreateInfoEXT
ValidationLayer::getDebugUtilsMessengerCreateInfoEXT() {
  vk::DebugUtilsMessengerCreateInfoEXT createInfo =
      vk::DebugUtilsMessengerCreateInfoEXT()
          .setMessageSeverity(Config::DEBUG_MESSENGER_SEVERITY)
          .setMessageType(Config::DEBUG_MESSAGE_TYPE)
          .setPfnUserCallback(ValidationLayer::debugCallback);

  return createInfo;
}
