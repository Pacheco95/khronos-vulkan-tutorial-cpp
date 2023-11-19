#include "BinaryLoader.hpp"

#include <spdlog/spdlog.h>

#include <fstream>

BinaryLoader::Buffer BinaryLoader::load(const std::string &filePath) {
  std::ifstream file(filePath, std::ios::ate | std::ios::binary);

  if (!file.is_open()) {
    throw std::runtime_error(
        fmt::format("Failed to open binary file {}", filePath)
    );
  }

  auto fileSize = file.tellg();
  std::vector<char> buffer(static_cast<std::vector<char>::size_type>(fileSize));

  file.seekg(0);
  file.read(buffer.data(), fileSize);

  file.close();

  SPDLOG_DEBUG("Binary file loaded {} ({}B)", filePath, fileSize);

  return buffer;
}
