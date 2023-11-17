#pragma once

#include "Window.hpp"

class Application {
 public:
  void run();

 private:
  Window m_window;

  void initWindow();
  void initVulkan();
  void mainLoop();
  void cleanup();
};
