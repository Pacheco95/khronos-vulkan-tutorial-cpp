#include <spdlog/spdlog.h>

#include <iostream>

#include "Application.hpp"

int main() {
  auto level = static_cast<spdlog::level::level_enum>(SPDLOG_ACTIVE_LEVEL);
  spdlog::set_level(level);

  Application app;

  try {
    app.run();
  } catch (const std::exception& e) {
    std::cerr << e.what() << std::endl;
    return EXIT_FAILURE;
  }

  return EXIT_SUCCESS;
}
