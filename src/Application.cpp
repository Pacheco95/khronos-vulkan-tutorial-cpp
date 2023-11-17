#include "Application.hpp"

#include "Config.hpp"
#include "Utils.hpp"

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

void Application::initVulkan() { createInstance(); }

void Application::mainLoop() {
  while (!m_window.shouldClose()) {
    m_window.pollEvents();
  }
}

void Application::cleanup() {
  m_instance.destroy();
}

void Application::createInstance() {
  vk::ApplicationInfo appInfo =
      vk::ApplicationInfo()
          .setPApplicationName(Config::APP_NAME)
          .setApplicationVersion(VK_MAKE_API_VERSION(0, 1, 0, 0))
          .setPEngineName("No Engine")
          .setEngineVersion(VK_MAKE_API_VERSION(0, 1, 0, 0))
          .setApiVersion(VK_API_VERSION_1_0);

  std::vector extension = Utils::getGlfwRequiredInstanceExtensions();

  vk::InstanceCreateInfo instanceInfo =
      vk::InstanceCreateInfo()
          .setPApplicationInfo(&appInfo)
          .setPEnabledExtensionNames(extension);

  m_instance = vk::createInstance(instanceInfo);
}
