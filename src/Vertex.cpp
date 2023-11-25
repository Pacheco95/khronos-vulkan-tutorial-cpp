#include "Vertex.hpp"

vk::VertexInputBindingDescription Vertex::getBindingDescription() {
  vk::VertexInputBindingDescription bindingDescription{};
  bindingDescription.binding = 0;
  bindingDescription.stride = sizeof(Vertex);
  bindingDescription.inputRate = vk::VertexInputRate::eVertex;
  return bindingDescription;
}

std::array<vk::VertexInputAttributeDescription, 3>
Vertex::getAttributeDescriptions() {
  const std::array attributeDescriptions{
      vk::VertexInputAttributeDescription()
          .setBinding(0)
          .setLocation(0)
          .setFormat(vk::Format::eR32G32B32Sfloat)
          .setOffset(offsetof(Vertex, pos)),

      vk::VertexInputAttributeDescription()
          .setBinding(0)
          .setLocation(1)
          .setFormat(vk::Format::eR32G32B32Sfloat)
          .setOffset(offsetof(Vertex, color)),

      vk::VertexInputAttributeDescription()
          .setBinding(0)
          .setLocation(2)
          .setFormat(vk::Format::eR32G32Sfloat)
          .setOffset(offsetof(Vertex, texCoord))};


  return attributeDescriptions;
}
