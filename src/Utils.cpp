#include "Utils.hpp"

#include <GLFW/glfw3.h>

std::vector<const char*> Utils::getGlfwRequiredInstanceExtensions() {
  uint32_t count = 0;
  const char** glfwExtensions = glfwGetRequiredInstanceExtensions(&count);
  std::vector<const char*> extensions(glfwExtensions, glfwExtensions + count);
  return extensions;
}
