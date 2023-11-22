#include "Window.hpp"

Window::~Window() {
  if (m_window) {
    glfwDestroyWindow(m_window);
  }

  glfwTerminate();
}

void Window::create(
    uint16_t width,
    uint16_t height,
    const std::string& title,
    const ResizeCallback& resizeCallback
) {
  if (m_window) {
    return;
  }

  glfwInit();
  glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);

  m_window = glfwCreateWindow(width, height, title.c_str(), nullptr, nullptr);
  m_resizeCallback = resizeCallback;

  glfwSetWindowUserPointer(m_window, this);
  glfwSetFramebufferSizeCallback(m_window, onFramebufferResize);
}

bool Window::shouldClose() const { return glfwWindowShouldClose(m_window); }

void Window::pollEvents() {  // NOLINT(*-convert-member-functions-to-static)
  glfwPollEvents();
}

vk::SurfaceKHR Window::createSurface(const vk::Instance& instance) const {
  VkSurfaceKHR surface;

  if (glfwCreateWindowSurface(instance, m_window, nullptr, &surface) !=
      VK_SUCCESS) {
    throw std::runtime_error("Failed to create window surface");
  }

  return surface;
}

vk::Extent2D Window::getExtent() const {
  int width, height;
  glfwGetFramebufferSize(m_window, &width, &height);

  vk::Extent2D actualExtent = {
      static_cast<uint32_t>(width), static_cast<uint32_t>(height)};

  return actualExtent;
}

bool Window::isMinimized() const {
  return glfwGetWindowAttrib(m_window, GLFW_ICONIFIED) == GLFW_TRUE;
}

void Window::onFramebufferResize(GLFWwindow* window, int width, int height) {
  const auto instance =
      reinterpret_cast<Window*>(glfwGetWindowUserPointer(window));

  if (instance->m_resizeCallback) {
    instance->m_resizeCallback(width, height);
  }
}
