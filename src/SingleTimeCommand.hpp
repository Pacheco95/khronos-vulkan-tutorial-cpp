#pragma once

#include <vulkan/vulkan.hpp>

class SingleTimeCommand {
 public:
  SingleTimeCommand(
      const vk::Device& device,
      const vk::Queue& mQueue,
      const vk::CommandPool& mCommandPool
  );

  ~SingleTimeCommand();

  vk::CommandBuffer& begin();

  void end();

 private:
  void free();

  const vk::Device& m_device;
  const vk::Queue& m_queue;
  const vk::CommandPool& m_commandPool;
  vk::CommandBuffer m_commandBuffer;
  bool m_ended = false;
};
