#include "Application.hpp"

#include <limits>

#include "Config.hpp"
#include "QueueFamily.hpp"
#include "Utils.hpp"
#include "ValidationLayer.hpp"

void Application::run() {
  initWindow();
  initVulkan();
  mainLoop();
  cleanup();
}

void Application::initWindow() {
  m_window.create(
      Config::WINDOW_WIDTH, Config::WINDOW_HEIGHT, Config::WINDOW_TITLE
  );
}

void Application::initVulkan() {
  createInstance();
  setupDebugMessenger();
  createSurface();
  pickPhysicalDevice();
  createLogicalDevice();
  createSwapChain();
}

void Application::mainLoop() {
  while (!m_window.shouldClose()) {
    m_window.pollEvents();
  }
}

void Application::cleanup() {
  m_device.destroy(m_swapChain);
  m_device.destroy();
  m_instance.destroy(m_surface);
  m_instance.destroy(m_debugMessenger);
  m_instance.destroy();
}

void Application::createInstance() {
  if (Config::IS_VALIDATION_LAYERS_ENABLED) {
    if (!ValidationLayer::checkValidationLayerSupport()) {
      throw std::runtime_error("Validation layers requested, but unavailable");
    }
  }

  vk::ApplicationInfo appInfo =
      vk::ApplicationInfo()
          .setPApplicationName(Config::APP_NAME)
          .setApplicationVersion(VK_MAKE_API_VERSION(0, 1, 0, 0))
          .setPEngineName("No Engine")
          .setEngineVersion(VK_MAKE_API_VERSION(0, 1, 0, 0))
          .setApiVersion(VK_API_VERSION_1_0);

  std::vector extensions = Utils::getRequiredInstanceExtensions();

  vk::InstanceCreateInfo instanceInfo =
      vk::InstanceCreateInfo()
          .setPApplicationInfo(&appInfo)
          .setPEnabledExtensionNames(extensions);

  vk::DebugUtilsMessengerCreateInfoEXT debugCreateInfo;

  if (Config::IS_VALIDATION_LAYERS_ENABLED) {
    debugCreateInfo = ValidationLayer::getDebugUtilsMessengerCreateInfoEXT();

    instanceInfo.pNext = &debugCreateInfo;
    instanceInfo.setPEnabledLayerNames(Config::VALIDATION_LAYERS);
  }

  m_instance = vk::createInstance(instanceInfo);
}

void Application::setupDebugMessenger() {
  if (!Config::IS_VALIDATION_LAYERS_ENABLED) {
    return;
  }

  ValidationLayer::loadDebugUtilsMessengerFunctions(m_instance);

  vk::DebugUtilsMessengerCreateInfoEXT createInfo =
      ValidationLayer::getDebugUtilsMessengerCreateInfoEXT();

  m_debugMessenger = m_instance.createDebugUtilsMessengerEXT(createInfo);
}

void Application::createSurface() {
  m_surface = m_window.createSurface(m_instance);
}

void Application::pickPhysicalDevice() {
  using namespace std;

  const vector<vk::PhysicalDevice>& devices =
      m_instance.enumeratePhysicalDevices();

  if (devices.empty()) {
    throw runtime_error("Failed to findOrNull devices with Vulkan support");
  }

  for (const auto& device : devices) {
    if (isDeviceSuitable(device)) {
      m_physicalDevice = device;
    }
  }

  if (!m_physicalDevice) {
    throw runtime_error("Failed to find a suitable GPU");
  }
}

void Application::createLogicalDevice() {
  const auto& indices = QueueFamily::findIndices(m_physicalDevice, m_surface);
  const auto& queueCreateInfos = indices.getQueueCreateInfos();
  vk::PhysicalDeviceFeatures deviceFeatures;

  const auto& deviceCreateInfo =
      vk::DeviceCreateInfo()
          .setQueueCreateInfos(queueCreateInfos)
          .setPEnabledFeatures(&deviceFeatures)
          .setPEnabledExtensionNames(Config::DEVICE_EXTENSIONS);

  m_device = m_physicalDevice.createDevice(deviceCreateInfo);

  m_graphicsQueue = m_device.getQueue(indices.graphicsFamily.value(), 0);
  m_presentQueue = m_device.getQueue(indices.presentFamily.value(), 0);
}

void Application::createSwapChain() {
  SwapChainSupportDetails swapChainSupport{m_physicalDevice, m_surface};

  const vk::SurfaceFormatKHR& surfaceFormat =
      chooseSwapSurfaceFormat(swapChainSupport.formats);

  const vk::PresentModeKHR& presentMode =
      chooseSwapPresentMode(swapChainSupport.presentModes);

  const vk::Extent2D& extent = chooseSwapExtent(swapChainSupport.capabilities);

  uint32_t imageCount = getSuitableImageCount(swapChainSupport);

  auto createInfo =
      vk::SwapchainCreateInfoKHR()
          .setSurface(m_surface)
          .setMinImageCount(imageCount)
          .setImageFormat(surfaceFormat.format)
          .setImageColorSpace(surfaceFormat.colorSpace)
          .setImageExtent(extent)
          .setImageArrayLayers(1)
          .setImageUsage(vk::ImageUsageFlagBits::eColorAttachment)
          .setPreTransform(swapChainSupport.capabilities.currentTransform)
          .setCompositeAlpha(vk::CompositeAlphaFlagBitsKHR::eOpaque)
          .setPresentMode(presentMode)
          .setClipped(vk::True);

  const auto& indices = QueueFamily::findIndices(m_physicalDevice, m_surface);

  uint32_t queueFamilyIndices[] = {
      indices.graphicsFamily.value(), indices.presentFamily.value()};

  if (indices.graphicsFamily == indices.presentFamily) {
    createInfo.imageSharingMode = vk::SharingMode::eExclusive;
  } else {
    createInfo.imageSharingMode = vk::SharingMode::eConcurrent;
    createInfo.setQueueFamilyIndices(queueFamilyIndices);
  }

  m_swapChain = m_device.createSwapchainKHR(createInfo);
  m_swapChainImages = m_device.getSwapchainImagesKHR(m_swapChain);
  m_swapChainImageFormat = surfaceFormat.format;
  m_swapChainExtent = extent;
}

bool Application::isDeviceSuitable(const vk::PhysicalDevice& device) const {
  const auto& indices = QueueFamily::findIndices(device, m_surface);
  bool extensionsSupported = checkDeviceExtensionSupport(device);

  bool suitableSwapChain;

  if (extensionsSupported) {
    SwapChainSupportDetails swapChainSupport =
        SwapChainSupportDetails(device, m_surface);

    suitableSwapChain = Utils::noneOf(
        swapChainSupport.formats.empty(), swapChainSupport.presentModes.empty()
    );
  }

  return indices.isComplete() && extensionsSupported && suitableSwapChain;
}

bool Application::checkDeviceExtensionSupport(const vk::PhysicalDevice& device
) {
  std::vector<vk::ExtensionProperties> availableExtensions =
      device.enumerateDeviceExtensionProperties();

  std::set<std::string> requiredExtensions(
      Config::DEVICE_EXTENSIONS.begin(), Config::DEVICE_EXTENSIONS.end()
  );

  for (const vk::ExtensionProperties& extension : availableExtensions) {
    requiredExtensions.erase(extension.extensionName);
  }

  return requiredExtensions.empty();
}

vk::SurfaceFormatKHR Application::chooseSwapSurfaceFormat(
    const std::vector<vk::SurfaceFormatKHR>& availableFormats
) {
  for (const auto& availableFormat : availableFormats) {
    if (availableFormat.format == vk::Format::eB8G8R8A8Srgb &&
        availableFormat.colorSpace == vk::ColorSpaceKHR::eSrgbNonlinear) {
      return availableFormat;
    }
  }

  return availableFormats[0];
}

vk::PresentModeKHR Application::chooseSwapPresentMode(
    const std::vector<vk::PresentModeKHR>& availablePresentModes
) {
  for (const auto& availablePresentMode : availablePresentModes) {
    if (availablePresentMode == vk::PresentModeKHR::eMailbox) {
      return availablePresentMode;
    }
  }

  return vk::PresentModeKHR::eFifo;
}

vk::Extent2D Application::chooseSwapExtent(
    const vk::SurfaceCapabilitiesKHR& capabilities
) const {
  if (capabilities.currentExtent.width !=
      std::numeric_limits<uint32_t>::max()) {
    return capabilities.currentExtent;
  }

  vk::Extent2D actualExtent = m_window.getExtent();

  actualExtent.width = std::clamp(
      actualExtent.width,
      capabilities.minImageExtent.width,
      capabilities.maxImageExtent.width
  );
  actualExtent.height = std::clamp(
      actualExtent.height,
      capabilities.minImageExtent.height,
      capabilities.maxImageExtent.height
  );

  return actualExtent;
}

uint32_t Application::getSuitableImageCount(
        const SwapChainSupportDetails& swapChainSupport
) {
    uint32_t requestImageCount = swapChainSupport.capabilities.minImageCount + 1;
    uint32_t maxImageCount = swapChainSupport.capabilities.maxImageCount;

    bool hasLimitedImageCount = maxImageCount > 0;

    if (hasLimitedImageCount && requestImageCount > maxImageCount) {
        requestImageCount = maxImageCount;
    }
    return requestImageCount;
}
