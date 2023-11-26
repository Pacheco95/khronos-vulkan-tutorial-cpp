#pragma once

#include <glm/glm.hpp>
#include <vulkan/vulkan.hpp>

#include "SingleTimeCommand.hpp"
#include "Vertex.hpp"
#include "Window.hpp"

class Application {
  struct SwapChainSupportDetails {
    vk::SurfaceCapabilitiesKHR capabilities;
    std::vector<vk::SurfaceFormatKHR> formats;
    std::vector<vk::PresentModeKHR> presentModes;

    SwapChainSupportDetails(
        const vk::PhysicalDevice& device, const vk::SurfaceKHR& surface
    ) {
      capabilities = device.getSurfaceCapabilitiesKHR(surface);
      formats = device.getSurfaceFormatsKHR(surface);
      presentModes = device.getSurfacePresentModesKHR(surface);
    }
  };

  struct UniformBufferObject {
    alignas(16) glm::mat4 model{};
    alignas(16) glm::mat4 view{};
    alignas(16) glm::mat4 proj{};
  };

 public:
  void run();

 private:
  Window m_window;
  vk::Instance m_instance;
  vk::DebugUtilsMessengerEXT m_debugMessenger;
  vk::SurfaceKHR m_surface;
  vk::PhysicalDevice m_physicalDevice;
  vk::Device m_device;
  vk::Queue m_graphicsQueue;  // Destroyed by logical device
  vk::Queue m_presentQueue;   // Destroyed by logical device
  vk::SwapchainKHR m_swapChain;
  std::vector<vk::Image> m_swapChainImages;  // Destroyed by swap chain
  vk::Format m_swapChainImageFormat;
  vk::Extent2D m_swapChainExtent;
  std::vector<vk::ImageView> m_swapChainImageViews;
  vk::RenderPass m_renderPass;
  vk::DescriptorSetLayout m_descriptorSetLayout;
  vk::PipelineLayout m_pipelineLayout;
  vk::Pipeline m_graphicsPipeline;
  std::vector<vk::Framebuffer> m_swapChainFrameBuffers;
  vk::CommandPool m_commandPool;

  vk::Image m_depthImage;
  vk::DeviceMemory m_depthImageMemory;
  vk::ImageView m_depthImageView;

  uint32_t m_mipLevels;
  vk::Image m_textureImage;
  vk::DeviceMemory m_textureImageMemory;
  vk::ImageView m_textureImageView;
  vk::Sampler m_textureSampler;

  std::vector<Vertex> m_vertices;
  std::vector<uint32_t> m_indices;

  vk::Buffer m_vertexBuffer;
  vk::DeviceMemory m_vertexBufferMemory;
  vk::Buffer m_indexBuffer;
  vk::DeviceMemory m_indexBufferMemory;

  std::vector<vk::Buffer> m_uniformBuffers;
  std::vector<vk::DeviceMemory> m_uniformBuffersMemory;
  std::vector<void*> m_uniformBuffersMapped;

  vk::DescriptorPool m_descriptorPool;
  std::vector<vk::DescriptorSet> m_descriptorSets;

  std::vector<vk::CommandBuffer> m_commandBuffers;  // Destroyed by command pool
  std::vector<vk::Semaphore> m_imageAvailableSemaphores;
  std::vector<vk::Semaphore> m_renderFinishedSemaphores;
  std::vector<vk::Fence> m_inFlightFences;
  uint32_t m_currentFrame = 0;
  bool m_framebufferResized = false;

  void initWindow();
  void initVulkan();
  void mainLoop();
  void drawFrame();
  void cleanup();

  void createInstance();
  void setupDebugMessenger();
  void createSurface();
  void pickPhysicalDevice();
  void createLogicalDevice();
  void createSwapChain();
  void recreateSwapChain();
  void cleanupSwapChain();
  void createImageViews();
  void createRenderPass();
  void createDescriptorSetLayout();
  void createGraphicsPipeline();
  void createCommandPool();
  void createDepthResources();
  void createFrameBuffers();
  void createTextureImage();
  void createTextureImageView();
  void createTextureSampler();
  void loadModel();
  void createVertexBuffer();
  void createIndexBuffer();
  void createUniformBuffers();
  void createDescriptorPool();
  void createDescriptorSets();
  void createCommandBuffers();
  void createSyncObjects();

  [[nodiscard]] bool isDeviceSuitable(const vk::PhysicalDevice& device) const;

  [[nodiscard]] static bool checkDeviceExtensionSupport(
      const vk::PhysicalDevice& device
  );

  [[nodiscard]] static vk::SurfaceFormatKHR chooseSwapSurfaceFormat(
      const std::vector<vk::SurfaceFormatKHR>& availableFormats
  );

  [[nodiscard]] static vk::PresentModeKHR chooseSwapPresentMode(
      const std::vector<vk::PresentModeKHR>& availablePresentModes
  );

  [[nodiscard]] vk::Extent2D chooseSwapExtent(
      const vk::SurfaceCapabilitiesKHR& capabilities
  ) const;

  [[nodiscard]] static uint32_t getSuitableImageCount(
      const SwapChainSupportDetails& swapChainSupport
  );

  [[nodiscard]] vk::ShaderModule createShaderModule(
      const std::vector<char>& code
  ) const;

  void recordCommandBuffer(
      const vk::CommandBuffer& commandBuffer, uint32_t imageIndex
  );

  uint32_t findMemoryType(
      uint32_t typeFilter, const vk::MemoryPropertyFlags& properties
  );

  void createBuffer(
      const vk::DeviceSize& size,
      const vk::BufferUsageFlags& usage,
      const vk::MemoryPropertyFlags& properties,
      vk::Buffer& buffer,
      vk::DeviceMemory& bufferMemory
  );

  void copyBuffer(
      const vk::Buffer& srcBuffer,
      vk::Buffer& dstBuffer,
      const vk::DeviceSize& size
  );

  void updateUniformBuffer(uint32_t currentImage);

  void createImage(
      uint32_t width,
      uint32_t height,
      uint32_t mipLevels,
      vk::Format format,
      vk::ImageTiling tiling,
      vk::ImageUsageFlags usage,
      vk::MemoryPropertyFlags properties,
      vk::Image& image,
      vk::DeviceMemory& imageMemory
  );

  void transitionImageLayout(
      vk::Image image,
      vk::Format format,
      vk::ImageLayout oldLayout,
      vk::ImageLayout newLayout,
      uint32_t mipLevels
  );

  void copyBufferToImage(
      vk::Buffer buffer, vk::Image image, uint32_t width, uint32_t height
  );

  vk::ImageView createImageView(
      vk::Image image,
      vk::Format format,
      vk::ImageAspectFlagBits aspectFlags,
      uint32_t mipLevels
  );

  vk::Format findSupportedFormat(
      const std::vector<vk::Format>& candidates,
      vk::ImageTiling tiling,
      vk::FormatFeatureFlags features
  );

  vk::Format findDepthFormat();

  void generateMipmaps(
      vk::Image image,
      vk::Format imageFormat,
      int32_t texWidth,
      int32_t texHeight,
      uint32_t mipLevels
  );

  SingleTimeCommand createSingleTimeCommand();
};
