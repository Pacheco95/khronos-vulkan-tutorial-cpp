#include "Config.hpp"

#ifdef NDEBUG
const bool Config::IS_VALIDATION_LAYERS_ENABLED = false;
#else
const bool Config::IS_VALIDATION_LAYERS_ENABLED = true;
#endif
