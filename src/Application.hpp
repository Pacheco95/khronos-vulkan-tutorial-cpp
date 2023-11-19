#pragma once

#include <vulkan/vulkan.hpp>

#include "Window.hpp"

class Application {
  struct SwapChainSupportDetails {
    vk::SurfaceCapabilitiesKHR capabilities;
    std::vector<vk::SurfaceFormatKHR> formats;
    std::vector<vk::PresentModeKHR> presentModes;

    SwapChainSupportDetails(vk::PhysicalDevice device, vk::SurfaceKHR surface) {
      capabilities = device.getSurfaceCapabilitiesKHR(surface);
      formats = device.getSurfaceFormatsKHR(surface);
      presentModes = device.getSurfacePresentModesKHR(surface);
    }
  };

 public:
  void run();

 private:
  Window m_window;
  vk::Instance m_instance;
  vk::DebugUtilsMessengerEXT m_debugMessenger;
  vk::SurfaceKHR m_surface;
  vk::PhysicalDevice m_physicalDevice;
  vk::Device m_device;
  vk::Queue m_graphicsQueue;  // Destroyed by logical device
  vk::Queue m_presentQueue;   // Destroyed by logical device
  vk::SwapchainKHR m_swapChain;
  std::vector<vk::Image> m_swapChainImages;  // Destroyed by swap chain
  vk::Format m_swapChainImageFormat;
  vk::Extent2D m_swapChainExtent;
  std::vector<vk::ImageView> m_swapChainImageViews;

  void initWindow();
  void initVulkan();
  void mainLoop();
  void cleanup();

  void createInstance();
  void setupDebugMessenger();
  void createSurface();
  void pickPhysicalDevice();
  void createLogicalDevice();
  void createSwapChain();
  void createImageViews();

  [[nodiscard]] bool isDeviceSuitable(const vk::PhysicalDevice& device) const;

  [[nodiscard]] static bool checkDeviceExtensionSupport(
      const vk::PhysicalDevice& device
  );

  [[nodiscard]] static vk::SurfaceFormatKHR chooseSwapSurfaceFormat(
      const std::vector<vk::SurfaceFormatKHR>& availableFormats
  );

  [[nodiscard]] static vk::PresentModeKHR chooseSwapPresentMode(
      const std::vector<vk::PresentModeKHR>& availablePresentModes
  );

  [[nodiscard]] vk::Extent2D chooseSwapExtent(
      const vk::SurfaceCapabilitiesKHR& capabilities
  ) const;

  [[nodiscard]] static uint32_t getSuitableImageCount(
      const SwapChainSupportDetails& swapChainSupport
  );
};
