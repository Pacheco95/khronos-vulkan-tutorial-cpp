#pragma once

#include <GLFW/glfw3.h>

#include <string>

class Window {
 public:
  Window() = default;
  Window(uint16_t width, uint16_t height, const std::string& title);
  ~Window();

  void create(uint16_t width, uint16_t height, const std::string& title);
  bool shouldClose();
  void pollEvents();

 private:
  GLFWwindow* m_window = nullptr;
};
