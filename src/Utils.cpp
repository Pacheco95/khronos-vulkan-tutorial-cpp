#include "Utils.hpp"

#include <GLFW/glfw3.h>

#include "Config.hpp"

std::vector<const char*> Utils::getRequiredInstanceExtensions() {
  uint32_t count = 0;
  const char** glfwExtensions = glfwGetRequiredInstanceExtensions(&count);
  std::vector<const char*> extensions(glfwExtensions, glfwExtensions + count);

  if (Config::IS_VALIDATION_LAYERS_ENABLED) {
    extensions.emplace_back(VK_EXT_DEBUG_UTILS_EXTENSION_NAME);
  }

  return extensions;
}
