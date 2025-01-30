#pragma once

#include <cstdint>
#include <string_view>
#include <vulkan/vulkan_core.h>

namespace engine::vulkan {

struct Version {
  uint32_t major;
  uint32_t minor;
  uint32_t patch;

  uint32_t vkMakeVersion() const {
    return VK_MAKE_VERSION(major, minor, patch);
  }
};

struct Config {
  uint32_t width;
  uint32_t height;
  std::string_view title;

  std::string_view appName;
  Version appVersion;
  std::string_view engineName;
  Version engineVersion;
  bool enableValidationLayers;
};

} // namespace engine::vulkan
