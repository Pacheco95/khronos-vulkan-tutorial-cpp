#pragma once

#include <vector>

struct Utils {
  static std::vector<const char*> getRequiredInstanceExtensions();

  template <typename... Args>
  static bool noneOf(Args... args) {
    return (... || !args);
  }
};
