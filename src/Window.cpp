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
