#pragma once

#include "config.hpp"

#include <vector>
#include <vulkan/vulkan_core.h>

namespace engine::vulkan {

class Instance {
public:
  Instance(const Config &config);
  ~Instance();

  VkInstance data() const { return instance; };

private:
  VkInstance instance;

  const Config &config;

  VkApplicationInfo getAppInfo() const;
  std::vector<const char *> getExtensions() const;
  VkDebugUtilsMessengerCreateInfoEXT getDebugMessengerCreateInfo();
  bool checkValidationLayerSupport() const;
};

} // namespace engine::vulkan
