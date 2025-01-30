#pragma once

#include "logicalDevice.hpp"
#include "swapChain.hpp"
#include <vulkan/vulkan_core.h>

namespace engine::vulkan {

class Device;

class Pipeline {
public:
  Pipeline(const LogicalDevice &logicalDevice, const SwapChain &swapChain,
           const RenderPass &renderPass, const Config &config);
  ~Pipeline();

  Pipeline(const Pipeline &) = delete;
  Pipeline &operator=(const Pipeline &) = delete;
  Pipeline(const Pipeline &&) = delete;
  Pipeline &operator=(const Pipeline &&) = delete;

  VkPipeline data() const { return pipeline; }

  std::string getShaderPath(const std::string &shaderFilename,
                            const Config &config) const;
  VkShaderModule createShaderModule(const std::vector<char> &code);

private:
  VkPipeline pipeline;
  VkPipelineLayout pipelineLayout;
  const LogicalDevice &logicalDevice;
};

} // namespace engine::vulkan
