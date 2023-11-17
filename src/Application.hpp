#pragma once

#include <vulkan/vulkan.hpp>

#include "Window.hpp"

class Application {
 public:
  void run();

 private:
  Window m_window;
  vk::Instance m_instance;

  void initWindow();
  void initVulkan();
  void mainLoop();
  void cleanup();

  void createInstance();
};
