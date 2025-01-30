#pragma once

#include "logicalDevice.hpp"

namespace engine::vulkan {

class RenderPass {
public:
  RenderPass(const LogicalDevice &logicalDevice);
  ~RenderPass();

  RenderPass(const RenderPass &) = delete;
  RenderPass &operator=(const RenderPass &) = delete;
  RenderPass(RenderPass &&) = delete;
  RenderPass &operator=(RenderPass &&) = delete;

  VkRenderPass data() const { return renderPass; }

private:
  VkRenderPass renderPass;
  const LogicalDevice &logicalDevice;
};

} // namespace engine::vulkan
