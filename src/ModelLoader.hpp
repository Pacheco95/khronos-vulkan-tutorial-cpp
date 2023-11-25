#pragma once

#include <string>

#include "Vertex.hpp"

class ModelLoader {
 public:
  static void loadObj(
      const std::string& modelPath,
      std::vector<Vertex>& vertices,
      std::vector<uint32_t>& indices
  );
};
