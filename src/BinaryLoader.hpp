#pragma once

#include <string>
#include <vector>

class BinaryLoader {
 public:
  typedef std::vector<char> Buffer;

  static Buffer load(const std::string& filePath);
};
