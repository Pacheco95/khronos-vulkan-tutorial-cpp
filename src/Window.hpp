#pragma once

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include <string>
#include <vulkan/vulkan.hpp>

class Window {
 public:
  Window() = default;
  ~Window();

  void create(uint16_t width, uint16_t height, const std::string& title);

  [[nodiscard]] bool shouldClose() const;

  void pollEvents();

  [[nodiscard]] vk::SurfaceKHR createSurface(const vk::Instance& instance
  ) const;

  [[nodiscard]] vk::Extent2D getExtent() const;

 private:
  GLFWwindow* m_window = nullptr;
};
