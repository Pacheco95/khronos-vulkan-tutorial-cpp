#pragma once

#include <array>
#include <glm/glm.hpp>
#include <vulkan/vulkan.hpp>

struct Vertex {
  glm::vec2 pos;
  glm::vec3 color;
  glm::vec2 texCoord;

  static vk::VertexInputBindingDescription getBindingDescription();

  static std::array<vk::VertexInputAttributeDescription, 3>
  getAttributeDescriptions();
};
