#include "Window.hpp"

Window::Window(uint16_t width, uint16_t height, const std::string& title) {
  create(width, height, title);
}

Window::~Window() {
  if (m_window) {
    glfwDestroyWindow(m_window);
  }

  glfwTerminate();
}

void Window::create(uint16_t width, uint16_t height, const std::string& title) {
  if (m_window) {
    return;
  }

  glfwInit();
  glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
  glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);

  m_window = glfwCreateWindow(width, height, title.c_str(), nullptr, nullptr);
}

bool Window::shouldClose() { return glfwWindowShouldClose(m_window); }

void Window::pollEvents() {  // NOLINT(*-convert-member-functions-to-static)
  glfwPollEvents();
}

vk::SurfaceKHR Window::createSurface(const vk::Instance& instance) {
  VkSurfaceKHR surface;

  if (glfwCreateWindowSurface(instance, m_window, nullptr, &surface) !=
      VK_SUCCESS) {
    throw std::runtime_error("Failed to create window surface");
  }

  return surface;
}

vk::Extent2D Window::getExtent() {
  int width, height;
  glfwGetFramebufferSize(m_window, &width, &height);

  vk::Extent2D actualExtent = {
      static_cast<uint32_t>(width), static_cast<uint32_t>(height)};

  return actualExtent;
}
