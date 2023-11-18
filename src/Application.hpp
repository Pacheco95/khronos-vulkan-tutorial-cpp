#pragma once

#include <vulkan/vulkan.hpp>

#include "Window.hpp"

class Application {
 public:
  void run();

 private:
  Window m_window;
  vk::Instance m_instance;
  vk::DebugUtilsMessengerEXT m_debugMessenger;
  vk::PhysicalDevice m_physicalDevice;

  void initWindow();
  void initVulkan();
  void mainLoop();
  void cleanup();

  void createInstance();
  void setupDebugMessenger();
  void pickPhysicalDevice();
  static bool isDeviceSuitable(const vk::PhysicalDevice& device);
};
