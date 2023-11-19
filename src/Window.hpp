#pragma once

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include <string>
#include <vulkan/vulkan.hpp>

class Window {
 public:
  Window() = default;
  Window(uint16_t width, uint16_t height, const std::string& title);
  ~Window();

  void create(uint16_t width, uint16_t height, const std::string& title);
  bool shouldClose();
  void pollEvents();
  vk::SurfaceKHR createSurface(const vk::Instance& instance);
  vk::Extent2D getExtent();

 private:
  GLFWwindow* m_window = nullptr;
};
