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
  vk::SurfaceKHR m_surface;
  vk::PhysicalDevice m_physicalDevice;
  vk::Device m_device;
  vk::Queue m_graphicsQueue;  // Automatically destroyed
  vk::Queue m_presentQueue;   // Automatically destroyed

  void initWindow();
  void initVulkan();
  void mainLoop();

  void cleanup();
  void createInstance();
  void setupDebugMessenger();
  void pickPhysicalDevice();
  void createLogicalDevice();
  void createSurface();

  bool isDeviceSuitable(const vk::PhysicalDevice& device);
};
