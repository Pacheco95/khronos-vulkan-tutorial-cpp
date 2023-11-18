#include "Application.hpp"

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
}

void Application::mainLoop() {
  while (!m_window.shouldClose()) {
    m_window.pollEvents();
  }
}

void Application::cleanup() {
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

  std::vector extension = Utils::getRequiredInstanceExtensions();

  vk::InstanceCreateInfo instanceInfo =
      vk::InstanceCreateInfo()
          .setPApplicationInfo(&appInfo)
          .setPEnabledExtensionNames(extension);

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
          .setPEnabledFeatures(&deviceFeatures);

  m_device = m_physicalDevice.createDevice(deviceCreateInfo);

  m_graphicsQueue = m_device.getQueue(indices.graphicsFamily.value(), 0);
  m_presentQueue = m_device.getQueue(indices.presentFamily.value(), 0);
}

void Application::createSurface() {
    m_surface = m_window.createSurface(m_instance);
}

bool Application::isDeviceSuitable(const vk::PhysicalDevice& device) {
  return QueueFamily::findIndices(device, m_surface).isComplete();
}
