#include "Application.hpp"

#include <limits>
#include <set>

#include "BinaryLoader.hpp"
#include "Config.hpp"
#include "QueueFamily.hpp"
#include "Utils.hpp"
#include "ValidationLayer.hpp"

constexpr auto NO_TIMEOUT = std::numeric_limits<uint64_t>::max();

void Application::run() {
  initWindow();
  initVulkan();
  mainLoop();
  cleanup();
}

void Application::initWindow() {
  m_window.create(
      Config::WINDOW_WIDTH, Config::WINDOW_HEIGHT, Config::WINDOW_TITLE
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
  createGraphicsPipeline();
  createFrameBuffers();
  createCommandPool();
  createCommandBuffer();
  createSyncObjects();
}

void Application::mainLoop() {
  while (!m_window.shouldClose()) {
    m_window.pollEvents();
    drawFrame();
  }

  m_device.waitIdle();
}

void Application::drawFrame() {
  (void)m_device.waitForFences(m_inFlightFence, vk::True, NO_TIMEOUT);
  m_device.resetFences(m_inFlightFence);


  vk::ResultValue acquireResult = m_device.acquireNextImageKHR(
      m_swapChain, NO_TIMEOUT, m_imageAvailableSemaphore
  );

  uint32_t imageIndex = acquireResult.value;

  m_commandBuffer.reset();
  recordCommandBuffer(m_commandBuffer, imageIndex);

  const vk::PipelineStageFlags waitStages{
      vk::PipelineStageFlagBits::eColorAttachmentOutput};

  const auto signalSemaphores = {m_renderFinishedSemaphore};

  const auto& submitInfo =
      vk::SubmitInfo()
          .setWaitSemaphores(m_imageAvailableSemaphore)
          .setWaitDstStageMask(waitStages)
          .setCommandBuffers(m_commandBuffer)
          .setSignalSemaphores(signalSemaphores);

  m_graphicsQueue.submit(submitInfo, m_inFlightFence);

  const auto& presentInfo =
      vk::PresentInfoKHR()
          .setWaitSemaphores(signalSemaphores)
          .setSwapchains(m_swapChain)
          .setImageIndices(imageIndex);

  (void)m_presentQueue.presentKHR(presentInfo);
}

void Application::cleanup() {
  m_device.destroy(m_inFlightFence);
  m_device.destroy(m_renderFinishedSemaphore);
  m_device.destroy(m_imageAvailableSemaphore);
  m_device.destroy(m_commandPool);
  for (const auto& frameBuffer : m_swapChainFrameBuffers) {
    m_device.destroy(frameBuffer);
  }
  m_device.destroy(m_graphicsPipeline);
  m_device.destroy(m_pipelineLayout);
  m_device.destroy(m_renderPass);
  for (const auto& imageView : m_swapChainImageViews) {
    m_device.destroy(imageView);
  }
  m_device.destroy(m_swapChain);
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
    throw runtime_error("Failed to findOrNull devices with Vulkan support");
  }

  for (const auto& device : devices) {
    if (isDeviceSuitable(device)) {
      m_physicalDevice = device;
    }
  }

  if (!m_physicalDevice) {
    throw runtime_error("Failed to find a suitable GPU");
  }
}

void Application::createLogicalDevice() {
  const auto& indices = QueueFamily::findIndices(m_physicalDevice, m_surface);
  const auto& queueCreateInfos = indices.getQueueCreateInfos();
  vk::PhysicalDeviceFeatures deviceFeatures;

  const auto& deviceCreateInfo =
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

  const auto& indices = QueueFamily::findIndices(m_physicalDevice, m_surface);

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

void Application::createImageViews() {
  m_swapChainImageViews.reserve(m_swapChainImages.size());

  for (const auto& m_swapChainImage : m_swapChainImages) {
    const auto& createInfo =
        vk::ImageViewCreateInfo()
            .setImage(m_swapChainImage)
            .setViewType(vk::ImageViewType::e2D)
            .setFormat(m_swapChainImageFormat)
            .setSubresourceRange(
                vk::ImageSubresourceRange()
                    .setAspectMask(vk::ImageAspectFlagBits::eColor)
                    .setBaseMipLevel(0)
                    .setLevelCount(1)
                    .setBaseArrayLayer(0)
                    .setLayerCount(1)
            );

    m_swapChainImageViews.emplace_back(m_device.createImageView(createInfo));
  }
}

void Application::createRenderPass() {
  const auto& colorAttachment =
      vk::AttachmentDescription()
          .setFormat(m_swapChainImageFormat)
          .setSamples(vk::SampleCountFlagBits::e1)
          .setLoadOp(vk::AttachmentLoadOp::eClear)
          .setStoreOp(vk::AttachmentStoreOp::eStore)
          .setStencilLoadOp(vk::AttachmentLoadOp::eDontCare)
          .setStencilStoreOp(vk::AttachmentStoreOp::eDontCare)
          .setInitialLayout(vk::ImageLayout::eUndefined)
          .setFinalLayout(vk::ImageLayout::ePresentSrcKHR);

  const auto& colorAttachmentRef =
      vk::AttachmentReference().setAttachment(0).setLayout(
          vk::ImageLayout::eColorAttachmentOptimal
      );

  const auto& subPass =
      vk::SubpassDescription()
          .setPipelineBindPoint(vk::PipelineBindPoint::eGraphics)
          .setColorAttachments(colorAttachmentRef);

  const auto& dependency =
      vk::SubpassDependency()
          .setSrcSubpass(vk::SubpassExternal)
          .setDstSubpass(0)
          .setSrcStageMask(vk::PipelineStageFlagBits::eColorAttachmentOutput)
          .setSrcAccessMask({})
          .setDstStageMask(vk::PipelineStageFlagBits::eColorAttachmentOutput)
          .setDstAccessMask(vk::AccessFlagBits::eColorAttachmentWrite);

  const auto& renderPassInfo =
      vk::RenderPassCreateInfo()
          .setAttachments(colorAttachment)
          .setSubpasses(subPass)
          .setDependencies(dependency);

  m_renderPass = m_device.createRenderPass(renderPassInfo);
}

void Application::createGraphicsPipeline() {
  auto vertShaderCode = BinaryLoader::load("res/shaders/shader.vert.spv");
  auto fragShaderCode = BinaryLoader::load("res/shaders/shader.frag.spv");

  vk::ShaderModule vertShaderModule = createShaderModule(vertShaderCode);
  vk::ShaderModule fragShaderModule = createShaderModule(fragShaderCode);

  const auto& vertShaderStageInfo =
      vk::PipelineShaderStageCreateInfo()
          .setStage(vk::ShaderStageFlagBits::eVertex)
          .setModule(vertShaderModule)
          .setPName("main");

  const auto& fragShaderStageInfo =
      vk::PipelineShaderStageCreateInfo()
          .setStage(vk::ShaderStageFlagBits::eFragment)
          .setModule(fragShaderModule)
          .setPName("main");

  vk::PipelineShaderStageCreateInfo shaderStages[]{
      vertShaderStageInfo, fragShaderStageInfo};

  const auto& vertexInputInfo =
      vk::PipelineVertexInputStateCreateInfo()
          .setVertexBindingDescriptionCount(0)
          .setVertexAttributeDescriptionCount(0);

  const auto& inputAssembly =
      vk::PipelineInputAssemblyStateCreateInfo()
          .setTopology(vk::PrimitiveTopology::eTriangleList)
          .setPrimitiveRestartEnable(vk::False);

  const auto& viewportState =
      vk::PipelineViewportStateCreateInfo().setViewportCount(1).setScissorCount(
          1
      );

  const auto& rasterizer =
      vk::PipelineRasterizationStateCreateInfo()
          .setDepthClampEnable(vk::False)
          .setRasterizerDiscardEnable(vk::False)
          .setPolygonMode(vk::PolygonMode::eFill)
          .setLineWidth(1.0f)
          .setCullMode(vk::CullModeFlagBits::eBack)
          .setFrontFace(vk::FrontFace::eClockwise)
          .setDepthBiasEnable(vk::False);

  const auto& multisampling =
      vk::PipelineMultisampleStateCreateInfo()
          .setSampleShadingEnable(vk::False)
          .setRasterizationSamples(vk::SampleCountFlagBits::e1);

  const auto& colorBlendAttachment =
      vk::PipelineColorBlendAttachmentState()
          .setColorWriteMask(
              vk::ColorComponentFlagBits::eR | vk::ColorComponentFlagBits::eG |
              vk::ColorComponentFlagBits::eB | vk::ColorComponentFlagBits::eA
          )
          .setBlendEnable(vk::False);

  const auto& colorBlending =
      vk::PipelineColorBlendStateCreateInfo()
          .setLogicOpEnable(vk::False)
          .setLogicOp(vk::LogicOp::eCopy)
          .setAttachments(colorBlendAttachment)
          .setBlendConstants({0.0f, 0.0f, 0.0f, 0.0f});

  std::vector dynamicStates = {
      vk::DynamicState::eViewport, vk::DynamicState::eScissor};

  const auto& dynamicState =
      vk::PipelineDynamicStateCreateInfo().setDynamicStates(dynamicStates);

  const auto& pipelineLayoutInfo =
      vk::PipelineLayoutCreateInfo()
          .setSetLayoutCount(0)
          .setPushConstantRangeCount(0);

  m_pipelineLayout = m_device.createPipelineLayout(pipelineLayoutInfo);

  const auto& pipelineInfo =
      vk::GraphicsPipelineCreateInfo()
          .setStages(shaderStages)
          .setPVertexInputState(&vertexInputInfo)
          .setPInputAssemblyState(&inputAssembly)
          .setPViewportState(&viewportState)
          .setPRasterizationState(&rasterizer)
          .setPMultisampleState(&multisampling)
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

void Application::createFrameBuffers() {
  m_swapChainFrameBuffers.resize(m_swapChainImageViews.size());

  for (size_t i = 0; i < m_swapChainImageViews.size(); ++i) {
    const auto& framebufferInfo =
        vk::FramebufferCreateInfo()
            .setRenderPass(m_renderPass)
            .setAttachments(m_swapChainImageViews[i])
            .setWidth(m_swapChainExtent.width)
            .setHeight(m_swapChainExtent.height)
            .setLayers(1);

    m_swapChainFrameBuffers[i] = m_device.createFramebuffer(framebufferInfo);
  }
}

void Application::createCommandPool() {
  const QueueFamily::Indices& queueFamilyIndices =
      QueueFamily::findIndices(m_physicalDevice, m_surface);

  const auto& poolInfo =
      vk::CommandPoolCreateInfo()
          .setFlags(vk::CommandPoolCreateFlagBits::eResetCommandBuffer)
          .setQueueFamilyIndex(queueFamilyIndices.graphicsFamily.value());

  m_commandPool = m_device.createCommandPool(poolInfo);
}

void Application::createCommandBuffer() {
  const auto& allocInfo =
      vk::CommandBufferAllocateInfo()
          .setCommandPool(m_commandPool)
          .setLevel(vk::CommandBufferLevel::ePrimary)
          .setCommandBufferCount(1);

  m_commandBuffer = m_device.allocateCommandBuffers(allocInfo).front();
}

void Application::createSyncObjects() {
  m_imageAvailableSemaphore = m_device.createSemaphore({});
  m_renderFinishedSemaphore = m_device.createSemaphore({});
  m_inFlightFence = m_device.createFence({vk::FenceCreateFlagBits::eSignaled});
}

bool Application::isDeviceSuitable(const vk::PhysicalDevice& device) const {
  const auto& indices = QueueFamily::findIndices(device, m_surface);
  bool extensionsSupported = checkDeviceExtensionSupport(device);

  bool suitableSwapChain = false;

  if (extensionsSupported) {
    SwapChainSupportDetails swapChainSupport =
        SwapChainSupportDetails(device, m_surface);

    suitableSwapChain = Utils::noneOf(
        swapChainSupport.formats.empty(), swapChainSupport.presentModes.empty()
    );
  }

  return indices.isComplete() && extensionsSupported && suitableSwapChain;
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

  vk::ClearValue clearColor{{0.0f, 0.0f, 0.0f, 1.0f}};
  vk::Offset2D zeroOffset{0, 0};

  const auto& renderPassInfo =
      vk::RenderPassBeginInfo()
          .setRenderPass(m_renderPass)
          .setFramebuffer(m_swapChainFrameBuffers[imageIndex])
          .setRenderArea({zeroOffset, m_swapChainExtent})
          .setClearValueCount(1)
          .setClearValues(clearColor);

  commandBuffer.beginRenderPass(renderPassInfo, vk::SubpassContents::eInline);

  commandBuffer.bindPipeline(
      vk::PipelineBindPoint::eGraphics, m_graphicsPipeline
  );

  const auto& viewport =
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
  commandBuffer.draw(3, 1, 0, 0);
  commandBuffer.endRenderPass();
  commandBuffer.end();
}
