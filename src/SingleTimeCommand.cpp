#include "SingleTimeCommand.hpp"

SingleTimeCommand::SingleTimeCommand(
    const vk::Device& device,
    const vk::Queue& mQueue,
    const vk::CommandPool& mCommandPool
)
    : m_device(device),
      m_queue(mQueue),
      m_commandPool(mCommandPool) {}

SingleTimeCommand::~SingleTimeCommand() {
  if (!m_ended) {
    end();
  }
}

vk::CommandBuffer& SingleTimeCommand::begin() {
  vk::CommandBufferAllocateInfo allocInfo;
  allocInfo.level = vk::CommandBufferLevel::ePrimary;
  allocInfo.commandPool = m_commandPool;
  allocInfo.commandBufferCount = 1;

  m_commandBuffer = m_device.allocateCommandBuffers(allocInfo)[0];

  vk::CommandBufferBeginInfo beginInfo;
  beginInfo.flags = vk::CommandBufferUsageFlagBits::eOneTimeSubmit;

  m_commandBuffer.begin(beginInfo);
  return m_commandBuffer;
}

void SingleTimeCommand::end() {
  m_ended = true;
  m_commandBuffer.end();

  vk::SubmitInfo submitInfo;
  submitInfo.setCommandBuffers(m_commandBuffer);

  m_queue.submit(submitInfo);
  m_queue.waitIdle();

  free();
}

void SingleTimeCommand::free() {
  m_device.freeCommandBuffers(m_commandPool, m_commandBuffer);
}
