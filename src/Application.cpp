#include "Application.hpp"

#include <stb_image.h>

#include <chrono>
#include <functional>
#include <glm/gtc/matrix_transform.hpp>
#include <limits>
#include <set>

#include "BinaryLoader.hpp"
#include "Config.hpp"
#include "MipmapGenerator.hpp"
#include "ModelLoader.hpp"
#include "QueueFamily.hpp"
#include "Utils.hpp"
#include "ValidationLayer.hpp"
#include "Vertex.hpp"

constexpr auto NO_TIMEOUT = std::numeric_limits<uint64_t>::max();
constexpr char MODEL_PATH[] = "res/models/viking_room.obj";
constexpr char VERTEX_SHADER_PATH[] = "res/shaders/shader.vert.spv";
constexpr char FRAGMENT_SHADER_PATH[] = "res/shaders/shader.frag.spv";

void Application::run() {
  initWindow();
  initVulkan();
  mainLoop();
  cleanup();
}

void Application::initWindow() {
  const auto resizeCallback =
      [this]([[maybe_unused]] int width, [[maybe_unused]] int height) {
        m_framebufferResized = true;
      };

  m_window.create(
      Config::WINDOW_WIDTH,
      Config::WINDOW_HEIGHT,
      Config::WINDOW_TITLE,
      resizeCallback
  );
}

void Application::initVulkan() {
  createInstance();
  setupDebugMessenger();
  createSurface();
  pickPhysicalDevice();
  createLogicalDevice();
  createSwapChain();
  createImageViews();
  createRenderPass();
  createDescriptorSetLayout();
  createGraphicsPipeline();
  createCommandPool();
  createColorResources();
  createDepthResources();
  createFrameBuffers();
  createTextureImage();
  createTextureImageView();
  createTextureSampler();
  loadModel();
  createVertexBuffer();
  createIndexBuffer();
  createUniformBuffers();
  createDescriptorPool();
  createDescriptorSets();
  createCommandBuffers();
  createSyncObjects();
}

void Application::mainLoop() {
  while (!m_window.shouldClose()) {
    m_window.pollEvents();

    if (!m_window.isMinimized()) {
      drawFrame();
    }
  }

  m_device.waitIdle();
}

void Application::drawFrame() {
  vk::Fence& inFlightFence = m_inFlightFences[m_currentFrame];
  vk::CommandBuffer& commandBuffer = m_commandBuffers[m_currentFrame];
  vk::Semaphore& imageAvailableSemaphore =
      m_imageAvailableSemaphores[m_currentFrame];
  vk::Semaphore& renderFinishedSemaphore =
      m_renderFinishedSemaphores[m_currentFrame];

  (void)m_device.waitForFences(inFlightFence, vk::True, NO_TIMEOUT);

  uint32_t imageIndex;
  vk::Result result;

  std::tie(result, imageIndex) = m_device.acquireNextImageKHR(
      m_swapChain, NO_TIMEOUT, imageAvailableSemaphore
  );

  if (result == vk::Result::eErrorOutOfDateKHR) {
    recreateSwapChain();
    return;
  }

  if (result != vk::Result::eSuccess && result != vk::Result::eSuboptimalKHR) {
    vk::resultCheck(result, "Failed to acquire swap chain image");
  }

  updateUniformBuffer(m_currentFrame);

  m_device.resetFences(inFlightFence);

  commandBuffer.reset();
  recordCommandBuffer(commandBuffer, imageIndex);

  const vk::PipelineStageFlags waitStages{
      vk::PipelineStageFlagBits::eColorAttachmentOutput};

  const auto signalSemaphores = {renderFinishedSemaphore};

  const auto submitInfo =
      vk::SubmitInfo()
          .setWaitSemaphores(imageAvailableSemaphore)
          .setWaitDstStageMask(waitStages)
          .setCommandBuffers(commandBuffer)
          .setSignalSemaphores(signalSemaphores);

  m_graphicsQueue.submit(submitInfo, inFlightFence);

  const auto presentInfo =
      vk::PresentInfoKHR()
          .setWaitSemaphores(signalSemaphores)
          .setSwapchains(m_swapChain)
          .setImageIndices(imageIndex);


  result = vk::Result(vkQueuePresentKHR(
      m_presentQueue, reinterpret_cast<const VkPresentInfoKHR*>(&presentInfo)
  ));

  if (result == vk::Result::eErrorOutOfDateKHR ||
      result == vk::Result::eSuboptimalKHR || m_framebufferResized) {
    m_framebufferResized = false;
    recreateSwapChain();
  } else {
    vk::resultCheck(result, "Failed to present swap chain image");
  }

  m_currentFrame = (m_currentFrame + 1) % Config::MAX_FRAMES_IN_FLIGHT;
}

void Application::cleanup() {
  cleanupSwapChain();

  m_device.destroy(m_descriptorPool);

  for (size_t i = 0; i < Config::MAX_FRAMES_IN_FLIGHT; ++i) {
    m_device.destroy(m_inFlightFences[i]);
    m_device.destroy(m_renderFinishedSemaphores[i]);
    m_device.destroy(m_imageAvailableSemaphores[i]);
  }

  for (size_t i = 0; i < Config::MAX_FRAMES_IN_FLIGHT; ++i) {
    m_device.destroy(m_uniformBuffers[i]);
    m_device.free(m_uniformBuffersMemory[i]);
  }

  m_device.destroy(m_indexBuffer);
  m_device.free(m_indexBufferMemory);

  m_device.destroy(m_vertexBuffer);
  m_device.free(m_vertexBufferMemory);

  m_device.destroy(m_textureSampler);
  m_device.destroy(m_textureImageView);

  m_device.free(m_textureImageMemory);
  m_device.destroy(m_textureImage);

  m_device.destroy(m_commandPool);
  m_device.destroy(m_graphicsPipeline);
  m_device.destroy(m_pipelineLayout);
  m_device.destroy(m_descriptorSetLayout);
  m_device.destroy(m_renderPass);
  m_device.destroy();
  m_instance.destroy(m_surface);
  m_instance.destroy(m_debugMessenger);
  m_instance.destroy();
}

void Application::createInstance() {
  if (Config::IS_VALIDATION_LAYERS_ENABLED) {
    if (!ValidationLayer::checkValidationLayerSupport()) {
      throw std::runtime_error("Validation layers requested, but unavailable");
    }
  }

  vk::ApplicationInfo appInfo =
      vk::ApplicationInfo()
          .setPApplicationName(Config::APP_NAME)
          .setApplicationVersion(VK_MAKE_API_VERSION(0, 1, 0, 0))
          .setPEngineName("No Engine")
          .setEngineVersion(VK_MAKE_API_VERSION(0, 1, 0, 0))
          .setApiVersion(VK_API_VERSION_1_0);

  std::vector extensions = Utils::getRequiredInstanceExtensions();

  vk::InstanceCreateInfo instanceInfo =
      vk::InstanceCreateInfo()
          .setPApplicationInfo(&appInfo)
          .setPEnabledExtensionNames(extensions);

  vk::DebugUtilsMessengerCreateInfoEXT debugCreateInfo;

  if (Config::IS_VALIDATION_LAYERS_ENABLED) {
    debugCreateInfo = ValidationLayer::getDebugUtilsMessengerCreateInfoEXT();

    instanceInfo.pNext = &debugCreateInfo;
    instanceInfo.setPEnabledLayerNames(Config::VALIDATION_LAYERS);
  }

  m_instance = vk::createInstance(instanceInfo);
}

void Application::setupDebugMessenger() {
  if (!Config::IS_VALIDATION_LAYERS_ENABLED) {
    return;
  }

  ValidationLayer::loadDebugUtilsMessengerFunctions(m_instance);

  vk::DebugUtilsMessengerCreateInfoEXT createInfo =
      ValidationLayer::getDebugUtilsMessengerCreateInfoEXT();

  m_debugMessenger = m_instance.createDebugUtilsMessengerEXT(createInfo);
}

void Application::createSurface() {
  m_surface = m_window.createSurface(m_instance);
}

void Application::pickPhysicalDevice() {
  using namespace std;

  const vector<vk::PhysicalDevice>& devices =
      m_instance.enumeratePhysicalDevices();

  if (devices.empty()) {
    throw runtime_error("Failed to find devices with Vulkan support");
  }

  for (const auto& device : devices) {
    if (isDeviceSuitable(device)) {
      m_physicalDevice = device;
      msaaSamples = getMaxUsableSampleCount();
      break;
    }
  }

  if (!m_physicalDevice) {
    throw runtime_error("Failed to find a suitable GPU");
  }
}

void Application::createLogicalDevice() {
  const auto indices = QueueFamily::findIndices(m_physicalDevice, m_surface);
  const auto queueCreateInfos = indices.getQueueCreateInfos();

  vk::PhysicalDeviceFeatures deviceFeatures;
  deviceFeatures.samplerAnisotropy = vk::True;

  const auto deviceCreateInfo =
      vk::DeviceCreateInfo()
          .setQueueCreateInfos(queueCreateInfos)
          .setPEnabledFeatures(&deviceFeatures)
          .setPEnabledExtensionNames(Config::DEVICE_EXTENSIONS);

  m_device = m_physicalDevice.createDevice(deviceCreateInfo);

  m_graphicsQueue = m_device.getQueue(indices.graphicsFamily.value(), 0);
  m_presentQueue = m_device.getQueue(indices.presentFamily.value(), 0);
}

void Application::createSwapChain() {
  SwapChainSupportDetails swapChainSupport{m_physicalDevice, m_surface};

  const vk::SurfaceFormatKHR& surfaceFormat =
      chooseSwapSurfaceFormat(swapChainSupport.formats);

  const vk::PresentModeKHR& presentMode =
      chooseSwapPresentMode(swapChainSupport.presentModes);

  const vk::Extent2D& extent = chooseSwapExtent(swapChainSupport.capabilities);

  uint32_t imageCount = getSuitableImageCount(swapChainSupport);

  auto createInfo =
      vk::SwapchainCreateInfoKHR()
          .setSurface(m_surface)
          .setMinImageCount(imageCount)
          .setImageFormat(surfaceFormat.format)
          .setImageColorSpace(surfaceFormat.colorSpace)
          .setImageExtent(extent)
          .setImageArrayLayers(1)
          .setImageUsage(vk::ImageUsageFlagBits::eColorAttachment)
          .setPreTransform(swapChainSupport.capabilities.currentTransform)
          .setCompositeAlpha(vk::CompositeAlphaFlagBitsKHR::eOpaque)
          .setPresentMode(presentMode)
          .setClipped(vk::True);

  const auto indices = QueueFamily::findIndices(m_physicalDevice, m_surface);

  uint32_t queueFamilyIndices[] = {
      indices.graphicsFamily.value(), indices.presentFamily.value()};

  if (indices.graphicsFamily == indices.presentFamily) {
    createInfo.imageSharingMode = vk::SharingMode::eExclusive;
  } else {
    createInfo.imageSharingMode = vk::SharingMode::eConcurrent;
    createInfo.setQueueFamilyIndices(queueFamilyIndices);
  }

  m_swapChain = m_device.createSwapchainKHR(createInfo);
  m_swapChainImages = m_device.getSwapchainImagesKHR(m_swapChain);
  m_swapChainImageFormat = surfaceFormat.format;
  m_swapChainExtent = extent;
}

void Application::recreateSwapChain() {
  m_device.waitIdle();

  cleanupSwapChain();

  createSwapChain();
  createImageViews();
  createColorResources();
  createDepthResources();
  createFrameBuffers();
}

void Application::cleanupSwapChain() {
  // Depth resources freed out of creation order because it needs to be
  // freed at swap chain recreation
  m_device.destroy(m_depthImageView);
  m_device.destroy(m_depthImage);
  m_device.free(m_depthImageMemory);

  vkDestroyImageView(m_device, colorImageView, nullptr);
  vkDestroyImage(m_device, colorImage, nullptr);
  vkFreeMemory(m_device, colorImageMemory, nullptr);


  for (const auto& frameBuffer : m_swapChainFrameBuffers) {
    m_device.destroy(frameBuffer);
  }

  for (const auto& imageView : m_swapChainImageViews) {
    m_device.destroy(imageView);
  }

  m_device.destroy(m_swapChain);
}

void Application::createImageViews() {
  m_swapChainImageViews.resize(m_swapChainImages.size());

  for (size_t i = 0; i < m_swapChainImages.size(); ++i) {
    m_swapChainImageViews[i] = createImageView(
        m_swapChainImages[i],
        m_swapChainImageFormat,
        vk::ImageAspectFlagBits::eColor,
        1
    );
  }
}

void Application::createRenderPass() {
  const auto colorAttachment =
      vk::AttachmentDescription()
          .setFormat(m_swapChainImageFormat)
          .setSamples(msaaSamples)
          .setLoadOp(vk::AttachmentLoadOp::eClear)
          .setStoreOp(vk::AttachmentStoreOp::eStore)
          .setStencilLoadOp(vk::AttachmentLoadOp::eDontCare)
          .setStencilStoreOp(vk::AttachmentStoreOp::eDontCare)
          .setInitialLayout(vk::ImageLayout::eUndefined)
          .setFinalLayout(vk::ImageLayout::eColorAttachmentOptimal);

  const auto depthAttachment =
      vk::AttachmentDescription()
          .setFormat(findDepthFormat())
          .setSamples(msaaSamples)
          .setLoadOp(vk::AttachmentLoadOp::eClear)
          .setStoreOp(vk::AttachmentStoreOp::eDontCare)
          .setStencilLoadOp(vk::AttachmentLoadOp::eDontCare)
          .setStencilStoreOp(vk::AttachmentStoreOp::eDontCare)
          .setInitialLayout(vk::ImageLayout::eUndefined)
          .setFinalLayout(vk::ImageLayout::eDepthStencilAttachmentOptimal);

  vk::AttachmentDescription colorAttachmentResolve{};
  colorAttachmentResolve.format = m_swapChainImageFormat;
  colorAttachmentResolve.samples = vk::SampleCountFlagBits::e1;
  colorAttachmentResolve.loadOp = vk::AttachmentLoadOp::eDontCare;
  colorAttachmentResolve.storeOp = vk::AttachmentStoreOp::eStore;
  colorAttachmentResolve.stencilLoadOp = vk::AttachmentLoadOp::eDontCare;
  colorAttachmentResolve.stencilStoreOp = vk::AttachmentStoreOp::eDontCare;
  colorAttachmentResolve.initialLayout = vk::ImageLayout::eUndefined;
  colorAttachmentResolve.finalLayout = vk::ImageLayout::ePresentSrcKHR;


  const auto colorAttachmentRef =
      vk::AttachmentReference().setAttachment(0).setLayout(
          vk::ImageLayout::eColorAttachmentOptimal
      );

  const auto depthAttachmentRef =
      vk::AttachmentReference().setAttachment(1).setLayout(
          vk::ImageLayout::eDepthStencilAttachmentOptimal
      );

  vk::AttachmentReference colorAttachmentResolveRef{};
  colorAttachmentResolveRef.attachment = 2;
  colorAttachmentResolveRef.layout = vk::ImageLayout::eColorAttachmentOptimal;

  const auto subPass =
      vk::SubpassDescription()
          .setPipelineBindPoint(vk::PipelineBindPoint::eGraphics)
          .setColorAttachments(colorAttachmentRef)
          .setPDepthStencilAttachment(&depthAttachmentRef)
          .setResolveAttachments(colorAttachmentResolveRef);

  const auto dependency =
      vk::SubpassDependency()
          .setSrcSubpass(vk::SubpassExternal)
          .setDstSubpass(0)
          .setSrcStageMask(
              vk::PipelineStageFlagBits::eColorAttachmentOutput |
              vk::PipelineStageFlagBits::eLateFragmentTests
          )
          .setSrcAccessMask(
              vk::AccessFlagBits::eColorAttachmentWrite |
              vk::AccessFlagBits::eDepthStencilAttachmentWrite
          )
          .setDstStageMask(
              vk::PipelineStageFlagBits::eColorAttachmentOutput |
              vk::PipelineStageFlagBits::eEarlyFragmentTests
          )
          .setDstAccessMask(
              vk::AccessFlagBits::eColorAttachmentWrite |
              vk::AccessFlagBits::eDepthStencilAttachmentWrite
          );

  const auto attachments = {
      colorAttachment, depthAttachment, colorAttachmentResolve};

  const auto renderPassInfo =
      vk::RenderPassCreateInfo()
          .setAttachments(attachments)
          .setSubpasses(subPass)
          .setDependencies(dependency);

  m_renderPass = m_device.createRenderPass(renderPassInfo);
}

void Application::createDescriptorSetLayout() {
  const auto uboLayoutBinding =
      vk::DescriptorSetLayoutBinding()
          .setBinding(0)
          .setDescriptorCount(1)
          .setDescriptorType(vk::DescriptorType::eUniformBuffer)
          .setStageFlags(vk::ShaderStageFlagBits::eVertex);

  const auto samplerLayoutBinding =
      vk::DescriptorSetLayoutBinding()
          .setBinding(1)
          .setDescriptorCount(1)
          .setDescriptorType(vk::DescriptorType::eCombinedImageSampler)
          .setStageFlags(vk::ShaderStageFlagBits::eFragment);


  std::array bindings = {uboLayoutBinding, samplerLayoutBinding};

  vk::DescriptorSetLayoutCreateInfo layoutInfo;
  layoutInfo.setBindings(bindings);

  m_descriptorSetLayout = m_device.createDescriptorSetLayout(layoutInfo);
}

void Application::createGraphicsPipeline() {
  auto vertShaderCode = BinaryLoader::load(VERTEX_SHADER_PATH);
  auto fragShaderCode = BinaryLoader::load(FRAGMENT_SHADER_PATH);

  vk::ShaderModule vertShaderModule = createShaderModule(vertShaderCode);
  vk::ShaderModule fragShaderModule = createShaderModule(fragShaderCode);

  const auto vertShaderStageInfo =
      vk::PipelineShaderStageCreateInfo()
          .setStage(vk::ShaderStageFlagBits::eVertex)
          .setModule(vertShaderModule)
          .setPName("main");

  const auto fragShaderStageInfo =
      vk::PipelineShaderStageCreateInfo()
          .setStage(vk::ShaderStageFlagBits::eFragment)
          .setModule(fragShaderModule)
          .setPName("main");

  vk::PipelineShaderStageCreateInfo shaderStages[]{
      vertShaderStageInfo, fragShaderStageInfo};

  auto bindingDescription = Vertex::getBindingDescription();
  auto attributeDescriptions = Vertex::getAttributeDescriptions();

  const auto vertexInputInfo =
      vk::PipelineVertexInputStateCreateInfo()
          .setVertexBindingDescriptions(bindingDescription)
          .setVertexAttributeDescriptions(attributeDescriptions);

  const auto inputAssembly =
      vk::PipelineInputAssemblyStateCreateInfo()
          .setTopology(vk::PrimitiveTopology::eTriangleList)
          .setPrimitiveRestartEnable(vk::False);

  const auto viewportState =
      vk::PipelineViewportStateCreateInfo().setViewportCount(1).setScissorCount(
          1
      );

  const auto rasterizer =
      vk::PipelineRasterizationStateCreateInfo()
          .setDepthClampEnable(vk::False)
          .setRasterizerDiscardEnable(vk::False)
          .setPolygonMode(vk::PolygonMode::eFill)
          .setLineWidth(1.0f)
          .setCullMode(vk::CullModeFlagBits::eBack)
          .setFrontFace(vk::FrontFace::eCounterClockwise)
          .setDepthBiasEnable(vk::False);

  const auto multisampling =
      vk::PipelineMultisampleStateCreateInfo()
          .setSampleShadingEnable(vk::False)
          .setRasterizationSamples(msaaSamples);

  const auto depthStencil =
      vk::PipelineDepthStencilStateCreateInfo()
          .setDepthTestEnable(vk::True)
          .setDepthWriteEnable(vk::True)
          .setDepthCompareOp(vk::CompareOp::eLess)
          .setDepthBoundsTestEnable(vk::False)
          .setStencilTestEnable(vk::False);


  const auto colorBlendAttachment =
      vk::PipelineColorBlendAttachmentState()
          .setColorWriteMask(
              vk::ColorComponentFlagBits::eR | vk::ColorComponentFlagBits::eG |
              vk::ColorComponentFlagBits::eB | vk::ColorComponentFlagBits::eA
          )
          .setBlendEnable(vk::False);

  const auto colorBlending =
      vk::PipelineColorBlendStateCreateInfo()
          .setLogicOpEnable(vk::False)
          .setLogicOp(vk::LogicOp::eCopy)
          .setAttachments(colorBlendAttachment)
          .setBlendConstants({0.0f, 0.0f, 0.0f, 0.0f});

  std::vector dynamicStates = {
      vk::DynamicState::eViewport, vk::DynamicState::eScissor};

  const auto dynamicState =
      vk::PipelineDynamicStateCreateInfo().setDynamicStates(dynamicStates);

  const auto pipelineLayoutInfo =
      vk::PipelineLayoutCreateInfo().setSetLayouts(m_descriptorSetLayout);

  m_pipelineLayout = m_device.createPipelineLayout(pipelineLayoutInfo);

  const auto pipelineInfo =
      vk::GraphicsPipelineCreateInfo()
          .setStages(shaderStages)
          .setPVertexInputState(&vertexInputInfo)
          .setPInputAssemblyState(&inputAssembly)
          .setPViewportState(&viewportState)
          .setPRasterizationState(&rasterizer)
          .setPMultisampleState(&multisampling)
          .setPDepthStencilState(&depthStencil)
          .setPColorBlendState(&colorBlending)
          .setPDynamicState(&dynamicState)
          .setLayout(m_pipelineLayout)
          .setRenderPass(m_renderPass)
          .setSubpass(0)
          .setBasePipelineHandle(VK_NULL_HANDLE);

  vk::Result createPipelineResult;

  std::tie(createPipelineResult, m_graphicsPipeline) =
      m_device.createGraphicsPipeline({}, pipelineInfo);

  vk::resultCheck(createPipelineResult, "Failed to create graphics pipeline");

  m_device.destroy(vertShaderModule);
  m_device.destroy(fragShaderModule);
}

void Application::createCommandPool() {
  const QueueFamily::Indices queueFamilyIndices =
      QueueFamily::findIndices(m_physicalDevice, m_surface);

  const auto poolInfo =
      vk::CommandPoolCreateInfo()
          .setFlags(vk::CommandPoolCreateFlagBits::eResetCommandBuffer)
          .setQueueFamilyIndex(queueFamilyIndices.graphicsFamily.value());

  m_commandPool = m_device.createCommandPool(poolInfo);
}

void Application::createColorResources() {
  createImage(
      m_swapChainExtent.width,
      m_swapChainExtent.height,
      1,
      msaaSamples,
      m_swapChainImageFormat,
      vk::ImageTiling::eOptimal,
      vk::ImageUsageFlagBits::eTransientAttachment |
          vk::ImageUsageFlagBits::eColorAttachment,
      vk::MemoryPropertyFlagBits::eDeviceLocal,
      colorImage,
      colorImageMemory
  );

  colorImageView = createImageView(
      colorImage, m_swapChainImageFormat, vk::ImageAspectFlagBits::eColor, 1
  );
}

void Application::createDepthResources() {
  vk::Format depthFormat = findDepthFormat();

  createImage(
      m_swapChainExtent.width,
      m_swapChainExtent.height,
      1,
      msaaSamples,
      depthFormat,
      vk::ImageTiling::eOptimal,
      vk::ImageUsageFlagBits::eDepthStencilAttachment,
      vk::MemoryPropertyFlagBits::eDeviceLocal,
      m_depthImage,
      m_depthImageMemory
  );

  m_depthImageView = createImageView(
      m_depthImage, depthFormat, vk::ImageAspectFlagBits::eDepth, 1
  );
}

void Application::createFrameBuffers() {
  m_swapChainFrameBuffers.resize(m_swapChainImageViews.size());

  for (size_t i = 0; i < m_swapChainImageViews.size(); ++i) {
    std::array attachments = {
        colorImageView, m_depthImageView, m_swapChainImageViews[i]};

    const auto framebufferInfo =
        vk::FramebufferCreateInfo()
            .setRenderPass(m_renderPass)
            .setAttachments(attachments)
            .setWidth(m_swapChainExtent.width)
            .setHeight(m_swapChainExtent.height)
            .setLayers(1);

    m_swapChainFrameBuffers[i] = m_device.createFramebuffer(framebufferInfo);
  }
}

void Application::createTextureImage() {
  using std::floor;
  using std::log2;
  using std::max;

  int texWidth, texHeight, texChannels;

  stbi_uc* pixels = stbi_load(
      "res/textures/viking_room.png",
      &texWidth,
      &texHeight,
      &texChannels,
      STBI_rgb_alpha
  );

  m_mipLevels =
      static_cast<uint32_t>(floor(log2(max(texWidth, texHeight)))) + 1;


  vk::DeviceSize imageSize = texWidth * texHeight * 4;

  if (!pixels) {
    throw std::runtime_error("failed to load texture image!");
  }

  vk::Buffer stagingBuffer;
  vk::DeviceMemory stagingBufferMemory;

  createBuffer(
      imageSize,
      vk::BufferUsageFlagBits::eTransferSrc,
      vk::MemoryPropertyFlagBits::eHostVisible |
          vk::MemoryPropertyFlagBits::eHostCoherent,
      stagingBuffer,
      stagingBufferMemory
  );

  void* data = m_device.mapMemory(stagingBufferMemory, 0, imageSize);
  memcpy(data, pixels, static_cast<size_t>(imageSize));
  m_device.unmapMemory(stagingBufferMemory);

  stbi_image_free(pixels);

  createImage(
      texWidth,
      texHeight,
      m_mipLevels,
      vk::SampleCountFlagBits::e1,
      vk::Format::eR8G8B8A8Srgb,
      vk::ImageTiling::eOptimal,
      vk::ImageUsageFlagBits::eTransferSrc |
          vk::ImageUsageFlagBits::eTransferDst |
          vk::ImageUsageFlagBits::eSampled,
      vk::MemoryPropertyFlagBits::eDeviceLocal,
      m_textureImage,
      m_textureImageMemory
  );

  transitionImageLayout(
      m_textureImage,
      vk::Format::eR8G8B8A8Srgb,
      vk::ImageLayout::eUndefined,
      vk::ImageLayout::eTransferDstOptimal,
      m_mipLevels
  );

  copyBufferToImage(
      stagingBuffer,
      m_textureImage,
      static_cast<uint32_t>(texWidth),
      static_cast<uint32_t>(texHeight)
  );

  auto singleTimeCommand = createSingleTimeCommand();

  MipmapGenerator::MipmapProps mipmapProps{
      m_textureImage,
      vk::Format::eR8G8B8A8Srgb,
      texWidth,
      texHeight,
      m_mipLevels};

  MipmapGenerator::generate(mipmapProps, singleTimeCommand, m_physicalDevice);

  m_device.destroy(stagingBuffer);
  m_device.free(stagingBufferMemory);
}

void Application::createTextureImageView() {
  m_textureImageView = createImageView(
      m_textureImage,
      vk::Format::eR8G8B8A8Srgb,
      vk::ImageAspectFlagBits::eColor,
      m_mipLevels
  );
}

void Application::createTextureSampler() {
  vk::PhysicalDeviceProperties properties = m_physicalDevice.getProperties();

  vk::SamplerCreateInfo samplerInfo;
  samplerInfo.magFilter = vk::Filter::eLinear;
  samplerInfo.minFilter = vk::Filter::eLinear;
  samplerInfo.addressModeU = vk::SamplerAddressMode::eRepeat;
  samplerInfo.addressModeV = vk::SamplerAddressMode::eRepeat;
  samplerInfo.addressModeW = vk::SamplerAddressMode::eRepeat;
  samplerInfo.anisotropyEnable = vk::True;
  samplerInfo.maxAnisotropy = properties.limits.maxSamplerAnisotropy;
  samplerInfo.borderColor = vk::BorderColor::eIntOpaqueBlack;
  samplerInfo.unnormalizedCoordinates = vk::False;
  samplerInfo.compareEnable = vk::False;
  samplerInfo.compareOp = vk::CompareOp::eAlways;
  samplerInfo.mipmapMode = vk::SamplerMipmapMode::eLinear;
  samplerInfo.mipmapMode = vk::SamplerMipmapMode::eLinear;
  samplerInfo.maxLod = vk::LodClampNone;

  m_textureSampler = m_device.createSampler(samplerInfo);
}

void Application::loadModel() {
  ModelLoader::loadObj(MODEL_PATH, m_vertices, m_indices);
}

void Application::createVertexBuffer() {
  size_t bufferSize = sizeof(m_vertices[0]) * m_vertices.size();

  vk::Buffer stagingBuffer;
  vk::DeviceMemory stagingBufferMemory;

  createBuffer(
      bufferSize,
      vk::BufferUsageFlagBits::eTransferSrc,
      vk::MemoryPropertyFlagBits::eHostVisible |
          vk::MemoryPropertyFlagBits::eHostCoherent,
      stagingBuffer,
      stagingBufferMemory
  );

  void* data = m_device.mapMemory(stagingBufferMemory, 0, bufferSize);
  memcpy(data, m_vertices.data(), static_cast<size_t>(bufferSize));
  m_device.unmapMemory(stagingBufferMemory);

  createBuffer(
      bufferSize,
      vk::BufferUsageFlagBits::eTransferDst |
          vk::BufferUsageFlagBits::eVertexBuffer,
      vk::MemoryPropertyFlagBits::eDeviceLocal,
      m_vertexBuffer,
      m_vertexBufferMemory
  );

  copyBuffer(stagingBuffer, m_vertexBuffer, bufferSize);

  m_device.destroy(stagingBuffer);
  m_device.free(stagingBufferMemory);
}

void Application::createIndexBuffer() {
  vk::DeviceSize bufferSize = sizeof(m_indices[0]) * m_indices.size();

  vk::Buffer stagingBuffer;
  vk::DeviceMemory stagingBufferMemory;

  createBuffer(
      bufferSize,
      vk::BufferUsageFlagBits::eTransferSrc,
      vk::MemoryPropertyFlagBits::eHostVisible |
          vk::MemoryPropertyFlagBits::eHostCoherent,
      stagingBuffer,
      stagingBufferMemory
  );

  void* data = m_device.mapMemory(stagingBufferMemory, 0, bufferSize, {});
  memcpy(data, m_indices.data(), static_cast<size_t>(bufferSize));
  m_device.unmapMemory(stagingBufferMemory);

  createBuffer(
      bufferSize,
      vk::BufferUsageFlagBits::eTransferDst |
          vk::BufferUsageFlagBits::eIndexBuffer,
      vk::MemoryPropertyFlagBits::eDeviceLocal,
      m_indexBuffer,
      m_indexBufferMemory
  );

  copyBuffer(stagingBuffer, m_indexBuffer, bufferSize);

  m_device.destroy(stagingBuffer, nullptr);
  m_device.free(stagingBufferMemory, nullptr);
}

void Application::createUniformBuffers() {
  vk::DeviceSize bufferSize = sizeof(UniformBufferObject);

  m_uniformBuffers.resize(Config::MAX_FRAMES_IN_FLIGHT);
  m_uniformBuffersMemory.resize(Config::MAX_FRAMES_IN_FLIGHT);
  m_uniformBuffersMapped.resize(Config::MAX_FRAMES_IN_FLIGHT);

  for (size_t i = 0; i < Config::MAX_FRAMES_IN_FLIGHT; ++i) {
    createBuffer(
        bufferSize,
        vk::BufferUsageFlagBits::eUniformBuffer,
        vk::MemoryPropertyFlagBits::eHostVisible |
            vk::MemoryPropertyFlagBits::eHostCoherent,
        m_uniformBuffers[i],
        m_uniformBuffersMemory[i]
    );

    m_uniformBuffersMapped[i] =
        m_device.mapMemory(m_uniformBuffersMemory[i], 0, bufferSize);
  }
}

void Application::createDescriptorPool() {
  const auto maxFramesInFlight =
      static_cast<uint32_t>(Config::MAX_FRAMES_IN_FLIGHT);

  std::array<vk::DescriptorPoolSize, 2> poolSizes;

  poolSizes[0].type = vk::DescriptorType::eUniformBuffer;
  poolSizes[0].descriptorCount = maxFramesInFlight;

  poolSizes[1].type = vk::DescriptorType::eCombinedImageSampler;
  poolSizes[1].descriptorCount = maxFramesInFlight;


  vk::DescriptorPoolCreateInfo poolInfo;
  poolInfo.setPoolSizes(poolSizes);
  poolInfo.maxSets = maxFramesInFlight;

  m_descriptorPool = m_device.createDescriptorPool(poolInfo);
}

void Application::createDescriptorSets() {
  const uint8_t framesInFlight = Config::MAX_FRAMES_IN_FLIGHT;

  std::vector<vk::DescriptorSetLayout> layouts(
      framesInFlight, m_descriptorSetLayout
  );

  vk::DescriptorSetAllocateInfo allocInfo;
  allocInfo.descriptorPool = m_descriptorPool;
  allocInfo.setSetLayouts(layouts);

  m_descriptorSets = m_device.allocateDescriptorSets(allocInfo);

  for (size_t i = 0; i < framesInFlight; ++i) {
    vk::DescriptorBufferInfo bufferInfo;
    bufferInfo.buffer = m_uniformBuffers[i];
    bufferInfo.offset = 0;
    bufferInfo.range = sizeof(UniformBufferObject);

    vk::DescriptorImageInfo imageInfo;
    imageInfo.imageLayout = vk::ImageLayout::eShaderReadOnlyOptimal;
    imageInfo.imageView = m_textureImageView;
    imageInfo.sampler = m_textureSampler;

    const std::array descriptorWrites{
        vk::WriteDescriptorSet()
            .setDstSet(m_descriptorSets[i])
            .setDstBinding(0)
            .setDstArrayElement(0)
            .setDescriptorType(vk::DescriptorType::eUniformBuffer)
            .setDescriptorCount(1)
            .setPBufferInfo(&bufferInfo),

        vk::WriteDescriptorSet()
            .setDstSet(m_descriptorSets[i])
            .setDstBinding(1)
            .setDstArrayElement(0)
            .setDescriptorType(vk::DescriptorType::eCombinedImageSampler)
            .setDescriptorCount(1)
            .setPImageInfo(&imageInfo)};

    m_device.updateDescriptorSets(descriptorWrites, {});
  }
}

void Application::createCommandBuffers() {
  m_commandBuffers.resize(Config::MAX_FRAMES_IN_FLIGHT);

  const auto allocInfo =
      vk::CommandBufferAllocateInfo()
          .setCommandPool(m_commandPool)
          .setLevel(vk::CommandBufferLevel::ePrimary)
          .setCommandBufferCount(Config::MAX_FRAMES_IN_FLIGHT);

  m_commandBuffers = m_device.allocateCommandBuffers(allocInfo);
}

void Application::createSyncObjects() {
  m_imageAvailableSemaphores.resize(Config::MAX_FRAMES_IN_FLIGHT);
  m_renderFinishedSemaphores.resize(Config::MAX_FRAMES_IN_FLIGHT);
  m_inFlightFences.resize(Config::MAX_FRAMES_IN_FLIGHT);

  vk::FenceCreateFlagBits fenceFlags = vk::FenceCreateFlagBits::eSignaled;

  for (size_t i = 0; i < Config::MAX_FRAMES_IN_FLIGHT; ++i) {
    m_imageAvailableSemaphores[i] = m_device.createSemaphore({});
    m_renderFinishedSemaphores[i] = m_device.createSemaphore({});
    m_inFlightFences[i] = m_device.createFence({fenceFlags});
  }
}

bool Application::isDeviceSuitable(const vk::PhysicalDevice& device) const {
  const auto indices = QueueFamily::findIndices(device, m_surface);
  bool extensionsSupported = checkDeviceExtensionSupport(device);

  bool suitableSwapChain = false;

  if (extensionsSupported) {
    SwapChainSupportDetails swapChainSupport =
        SwapChainSupportDetails(device, m_surface);

    suitableSwapChain = Utils::noneOf(
        swapChainSupport.formats.empty(), swapChainSupport.presentModes.empty()
    );
  }

  vk::PhysicalDeviceFeatures supportedFeatures = device.getFeatures();

  return indices.isComplete() && extensionsSupported && suitableSwapChain &&
         supportedFeatures.samplerAnisotropy;
}

bool Application::checkDeviceExtensionSupport(const vk::PhysicalDevice& device
) {
  std::vector<vk::ExtensionProperties> availableExtensions =
      device.enumerateDeviceExtensionProperties();

  std::set<std::string> requiredExtensions(
      Config::DEVICE_EXTENSIONS.begin(), Config::DEVICE_EXTENSIONS.end()
  );

  for (const vk::ExtensionProperties& extension : availableExtensions) {
    requiredExtensions.erase(extension.extensionName);
  }

  return requiredExtensions.empty();
}

vk::SurfaceFormatKHR Application::chooseSwapSurfaceFormat(
    const std::vector<vk::SurfaceFormatKHR>& availableFormats
) {
  for (const auto& availableFormat : availableFormats) {
    if (availableFormat.format == vk::Format::eB8G8R8A8Srgb &&
        availableFormat.colorSpace == vk::ColorSpaceKHR::eSrgbNonlinear) {
      return availableFormat;
    }
  }

  return availableFormats[0];
}

vk::PresentModeKHR Application::chooseSwapPresentMode(
    const std::vector<vk::PresentModeKHR>& availablePresentModes
) {
  for (const auto& availablePresentMode : availablePresentModes) {
    if (availablePresentMode == vk::PresentModeKHR::eMailbox) {
      return availablePresentMode;
    }
  }

  return vk::PresentModeKHR::eFifo;
}

vk::Extent2D Application::chooseSwapExtent(
    const vk::SurfaceCapabilitiesKHR& capabilities
) const {
  if (capabilities.currentExtent.width !=
      std::numeric_limits<uint32_t>::max()) {
    return capabilities.currentExtent;
  }

  vk::Extent2D actualExtent = m_window.getExtent();

  actualExtent.width = std::clamp(
      actualExtent.width,
      capabilities.minImageExtent.width,
      capabilities.maxImageExtent.width
  );
  actualExtent.height = std::clamp(
      actualExtent.height,
      capabilities.minImageExtent.height,
      capabilities.maxImageExtent.height
  );

  return actualExtent;
}

uint32_t Application::getSuitableImageCount(
    const SwapChainSupportDetails& swapChainSupport
) {
  uint32_t requestImageCount = swapChainSupport.capabilities.minImageCount + 1;
  uint32_t maxImageCount = swapChainSupport.capabilities.maxImageCount;

  bool hasLimitedImageCount = maxImageCount > 0;

  if (hasLimitedImageCount && requestImageCount > maxImageCount) {
    requestImageCount = maxImageCount;
  }
  return requestImageCount;
}

vk::ShaderModule Application::createShaderModule(const std::vector<char>& code
) const {
  vk::ShaderModuleCreateInfo createInfo;
  createInfo.codeSize = code.size();
  createInfo.pCode = reinterpret_cast<const uint32_t*>(code.data());
  vk::ShaderModule shaderModule = m_device.createShaderModule(createInfo);
  return shaderModule;
}

void Application::recordCommandBuffer(
    const vk::CommandBuffer& commandBuffer, uint32_t imageIndex
) {
  vk::CommandBufferBeginInfo beginInfo;

  commandBuffer.begin(beginInfo);

  vk::Offset2D zeroOffset{0, 0};

  std::array<vk::ClearValue, 2> clearValues{};
  clearValues[0].color = vk::ClearColorValue{0.0f, 0.0f, 0.0f, 1.0f};
  clearValues[1].depthStencil = vk::ClearDepthStencilValue{1.0f, 0};

  const auto renderPassInfo =
      vk::RenderPassBeginInfo()
          .setRenderPass(m_renderPass)
          .setFramebuffer(m_swapChainFrameBuffers[imageIndex])
          .setRenderArea({zeroOffset, m_swapChainExtent})
          .setClearValues(clearValues);

  commandBuffer.beginRenderPass(renderPassInfo, vk::SubpassContents::eInline);

  commandBuffer.bindPipeline(
      vk::PipelineBindPoint::eGraphics, m_graphicsPipeline
  );

  const auto viewport =
      vk::Viewport()
          .setX(0.0f)
          .setY(0.0f)
          .setWidth(static_cast<float>(m_swapChainExtent.width))
          .setHeight(static_cast<float>(m_swapChainExtent.height))
          .setMinDepth(0.0f)
          .setMaxDepth(1.0f);

  commandBuffer.setViewport(0, viewport);

  vk::Rect2D scissor = {zeroOffset, m_swapChainExtent};

  commandBuffer.setScissor(0, scissor);
  commandBuffer.bindVertexBuffers(0, m_vertexBuffer, {0});
  commandBuffer.bindIndexBuffer(m_indexBuffer, 0, vk::IndexType::eUint32);
  commandBuffer.bindDescriptorSets(
      vk::PipelineBindPoint::eGraphics,
      m_pipelineLayout,
      0,
      m_descriptorSets[m_currentFrame],
      {}
  );
  commandBuffer.drawIndexed(
      static_cast<uint32_t>(m_indices.size()), 1, 0, 0, 0
  );
  commandBuffer.endRenderPass();
  commandBuffer.end();
}

uint32_t Application::findMemoryType(
    uint32_t typeFilter, const vk::MemoryPropertyFlags& properties
) {
  vk::PhysicalDeviceMemoryProperties memProperties =
      m_physicalDevice.getMemoryProperties();

  for (uint32_t i = 0; i < memProperties.memoryTypeCount; ++i) {
    if ((typeFilter & (1 << i)) && (memProperties.memoryTypes[i].propertyFlags &
                                    properties) == properties) {
      return i;
    }
  }

  throw std::runtime_error("Failed to find suitable memory type");
}

void Application::createBuffer(
    const vk::DeviceSize& size,
    const vk::BufferUsageFlags& usage,
    const vk::MemoryPropertyFlags& properties,
    vk::Buffer& buffer,
    vk::DeviceMemory& bufferMemory
) {
  vk::BufferCreateInfo bufferInfo;
  bufferInfo.size = size;
  bufferInfo.usage = usage;
  bufferInfo.sharingMode = vk::SharingMode::eExclusive;

  buffer = m_device.createBuffer(bufferInfo);

  vk::MemoryRequirements memoryRequirements =
      m_device.getBufferMemoryRequirements(buffer);

  vk::MemoryAllocateInfo allocInfo;
  allocInfo.allocationSize = memoryRequirements.size;
  allocInfo.memoryTypeIndex =
      findMemoryType(memoryRequirements.memoryTypeBits, properties);

  bufferMemory = m_device.allocateMemory(allocInfo);

  m_device.bindBufferMemory(buffer, bufferMemory, 0);
}

void Application::copyBuffer(
    const vk::Buffer& srcBuffer,
    vk::Buffer& dstBuffer,
    const vk::DeviceSize& size
) {
  auto singleTimeCommand = createSingleTimeCommand();
  vk::CommandBuffer commandBuffer = singleTimeCommand.begin();

  vk::BufferCopy copyRegion;
  copyRegion.size = size;

  commandBuffer.copyBuffer(srcBuffer, dstBuffer, 1, &copyRegion);
}

void Application::updateUniformBuffer(uint32_t currentImage) {
  using namespace std::chrono;
  using period = seconds::period;

  static auto startTime = high_resolution_clock::now();

  auto currentTime = high_resolution_clock::now();
  float time = duration<float, period>(currentTime - startTime).count();

  UniformBufferObject ubo;

  ubo.model = glm::rotate(
      glm::mat4(1.0f), time * glm::radians(90.0f), glm::vec3(0.0f, 0.0f, 1.0f)
  );

  ubo.view = glm::lookAt(
      glm::vec3(2.0f, 2.0f, 2.0f),
      glm::vec3(0.0f, 0.0f, 0.0f),
      glm::vec3(0.0f, 0.0f, 1.0f)
  );

  float aspect = static_cast<float>(m_swapChainExtent.width) /
                 static_cast<float>(m_swapChainExtent.height);

  ubo.proj = glm::perspective(glm::radians(45.0f), aspect, 0.1f, 10.0f);
  ubo.proj[1][1] *= -1;

  std::memcpy(m_uniformBuffersMapped[currentImage], &ubo, sizeof(ubo));
}

void Application::createImage(
    uint32_t width,
    uint32_t height,
    uint32_t mipLevels,
    vk::SampleCountFlagBits numSamples,
    vk::Format format,
    vk::ImageTiling tiling,
    vk::ImageUsageFlags usage,
    vk::MemoryPropertyFlags properties,
    vk::Image& image,
    vk::DeviceMemory& imageMemory
) {
  const auto imageInfo =
      vk::ImageCreateInfo()
          .setImageType(vk::ImageType::e2D)
          .setExtent({width, height, 1})
          .setMipLevels(mipLevels)
          .setArrayLayers(1)
          .setFormat(format)
          .setTiling(tiling)
          .setInitialLayout(vk::ImageLayout::eUndefined)
          .setUsage(usage)
          .setSamples(numSamples)
          .setSharingMode(vk::SharingMode::eExclusive);

  image = m_device.createImage(imageInfo);

  vk::MemoryRequirements memRequirements =
      m_device.getImageMemoryRequirements(image);

  vk::MemoryAllocateInfo allocInfo;
  allocInfo.allocationSize = memRequirements.size;
  allocInfo.memoryTypeIndex =
      findMemoryType(memRequirements.memoryTypeBits, properties);

  imageMemory = m_device.allocateMemory(allocInfo);
  m_device.bindImageMemory(image, imageMemory, 0);
}

void Application::transitionImageLayout(
    vk::Image image,
    vk::Format format,
    vk::ImageLayout oldLayout,
    vk::ImageLayout newLayout,
    uint32_t mipLevels
) {
  auto singleTimeCommand = createSingleTimeCommand();
  vk::CommandBuffer& commandBuffer = singleTimeCommand.begin();

  vk::ImageMemoryBarrier barrier;
  barrier.oldLayout = oldLayout;
  barrier.newLayout = newLayout;
  barrier.srcQueueFamilyIndex = vk::QueueFamilyIgnored;
  barrier.dstQueueFamilyIndex = vk::QueueFamilyIgnored;
  barrier.image = image;
  barrier.subresourceRange.aspectMask = vk::ImageAspectFlagBits::eColor;
  barrier.subresourceRange.baseMipLevel = 0;
  barrier.subresourceRange.levelCount = mipLevels;
  barrier.subresourceRange.baseArrayLayer = 0;
  barrier.subresourceRange.layerCount = 1;

  vk::PipelineStageFlags sourceStage, destinationStage;

  if (oldLayout == vk::ImageLayout::eUndefined &&
      newLayout == vk::ImageLayout::eTransferDstOptimal) {
    barrier.srcAccessMask = {};
    barrier.dstAccessMask = vk::AccessFlagBits::eTransferWrite;

    sourceStage = vk::PipelineStageFlagBits::eTopOfPipe;
    destinationStage = vk::PipelineStageFlagBits::eTransfer;
  } else if (oldLayout == vk::ImageLayout::eTransferDstOptimal && newLayout == vk::ImageLayout::eShaderReadOnlyOptimal) {
    barrier.srcAccessMask = vk::AccessFlagBits::eTransferWrite;
    barrier.dstAccessMask = vk::AccessFlagBits::eShaderRead;

    sourceStage = vk::PipelineStageFlagBits::eTransfer;
    destinationStage = vk::PipelineStageFlagBits::eFragmentShader;
  } else {
    throw std::invalid_argument("unsupported layout transition!");
  }

  commandBuffer.pipelineBarrier(
      sourceStage,
      destinationStage,
      vk::DependencyFlags(),
      0,
      nullptr,
      0,
      nullptr,
      1,
      &barrier
  );
}

void Application::copyBufferToImage(
    vk::Buffer buffer, vk::Image image, uint32_t width, uint32_t height
) {
  auto singleTimeCommand = createSingleTimeCommand();
  vk::CommandBuffer commandBuffer = singleTimeCommand.begin();

  vk::BufferImageCopy region;
  region.bufferOffset = 0;
  region.bufferRowLength = 0;
  region.bufferImageHeight = 0;
  region.imageSubresource.aspectMask = vk::ImageAspectFlagBits::eColor;
  region.imageSubresource.mipLevel = 0;
  region.imageSubresource.baseArrayLayer = 0;
  region.imageSubresource.layerCount = 1;
  region.imageOffset = vk::Offset3D{0, 0, 0};
  region.imageExtent = vk::Extent3D{width, height, 1};

  commandBuffer.copyBufferToImage(
      buffer, image, vk::ImageLayout::eTransferDstOptimal, 1, &region
  );
}

vk::ImageView Application::createImageView(
    vk::Image image,
    vk::Format format,
    vk::ImageAspectFlagBits aspectFlags,
    uint32_t mipLevels
) {
  vk::ImageViewCreateInfo viewInfo;
  viewInfo.image = image;
  viewInfo.viewType = vk::ImageViewType::e2D;
  viewInfo.format = format;
  viewInfo.subresourceRange.aspectMask = aspectFlags;
  viewInfo.subresourceRange.baseMipLevel = 0;
  viewInfo.subresourceRange.levelCount = mipLevels;
  viewInfo.subresourceRange.baseArrayLayer = 0;
  viewInfo.subresourceRange.layerCount = 1;

  return m_device.createImageView(viewInfo);
}

vk::Format Application::findSupportedFormat(
    const std::vector<vk::Format>& candidates,
    vk::ImageTiling tiling,
    vk::FormatFeatureFlags features
) {
  for (vk::Format format : candidates) {
    vk::FormatProperties props = m_physicalDevice.getFormatProperties(format);

    if (tiling == vk::ImageTiling::eLinear &&
        (props.linearTilingFeatures & features) == features) {
      return format;
    }

    if (tiling == vk::ImageTiling::eOptimal &&
        (props.optimalTilingFeatures & features) == features) {
      return format;
    }
  }

  throw std::runtime_error("Failed to find supported format");
}

vk::Format Application::findDepthFormat() {
  return findSupportedFormat(
      {vk::Format::eD32Sfloat,
       vk::Format::eD32SfloatS8Uint,
       vk::Format::eD24UnormS8Uint},
      vk::ImageTiling::eOptimal,
      vk::FormatFeatureFlagBits::eDepthStencilAttachment
  );
}

SingleTimeCommand Application::createSingleTimeCommand() {
  return SingleTimeCommand(m_device, m_graphicsQueue, m_commandPool);
}
