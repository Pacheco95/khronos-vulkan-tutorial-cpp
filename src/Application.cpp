#include "Application.hpp"

#include "Config.hpp"

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

void Application::initVulkan() {}

void Application::mainLoop() {
  while (!m_window.shouldClose()) {
    m_window.pollEvents();
  }
}

void Application::cleanup() {}
