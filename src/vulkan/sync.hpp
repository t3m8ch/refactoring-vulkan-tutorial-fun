#pragma once

#include "logicalDevice.hpp"

#include <vector>
#include <vulkan/vulkan_core.h>

namespace engine::vulkan {

class Sync {
public:
  Sync(const LogicalDevice &logicalDevice);
  ~Sync();

  const std::vector<VkSemaphore> &getImageAvailableSemaphores() const {
    return imageAvailableSemaphores;
  }

  const std::vector<VkSemaphore> &getRenderFinishedSemaphores() const {
    return renderFinishedSemaphores;
  }

  const std::vector<VkFence> &getInFlightFences() const {
    return inFlightFences;
  }

private:
  std::vector<VkSemaphore> imageAvailableSemaphores;
  std::vector<VkSemaphore> renderFinishedSemaphores;
  std::vector<VkFence> inFlightFences;

  const LogicalDevice &logicalDevice;
};

} // namespace engine::vulkan
