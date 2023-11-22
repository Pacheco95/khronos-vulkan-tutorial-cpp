#pragma once

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include <functional>
#include <string>
#include <vulkan/vulkan.hpp>

class Window {
 public:
  typedef std::function<void(int, int)> ResizeCallback;

  Window() = default;
  ~Window();

  void create(
      uint16_t width,
      uint16_t height,
      const std::string& title,
      const ResizeCallback& resizeCallback = {}
  );

  [[nodiscard]] bool shouldClose() const;

  void pollEvents();

  [[nodiscard]] vk::SurfaceKHR createSurface(const vk::Instance& instance
  ) const;

  [[nodiscard]] vk::Extent2D getExtent() const;

  [[nodiscard]] bool isMinimized() const;

 private:
  GLFWwindow* m_window = nullptr;
  ResizeCallback m_resizeCallback{};

  static void onFramebufferResize(GLFWwindow* window, int width, int height);
};
