#pragma once

#include <array>
#include <glm/glm.hpp>
#include <vulkan/vulkan.hpp>

struct Vertex {
  glm::vec3 pos;
  glm::vec3 color;
  glm::vec2 texCoord;

  static vk::VertexInputBindingDescription getBindingDescription();

  static std::array<vk::VertexInputAttributeDescription, 3>
  getAttributeDescriptions();

  bool operator==(const Vertex& other) const {
    return pos == other.pos && color == other.color &&
           texCoord == other.texCoord;
  }
};
