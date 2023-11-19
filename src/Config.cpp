#include "Config.hpp"

#ifdef NDEBUG
const bool Config::IS_VALIDATION_LAYERS_ENABLED = false;
#else
const bool Config::IS_VALIDATION_LAYERS_ENABLED = true;
#endif

const std::vector<const char*> Config::DEVICE_EXTENSIONS{
    VK_KHR_SWAPCHAIN_EXTENSION_NAME};
