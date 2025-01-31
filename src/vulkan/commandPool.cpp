#include "commandPool.hpp"
#include "constants.hpp"
#include "logicalDevice.hpp"

namespace engine::vulkan {

CommandPool::CommandPool(const LogicalDevice &logicalDevice,
                         const RenderPass &renderPass,
                         const SwapChain &swapChain, const Pipeline &pipeline)
    : logicalDevice(logicalDevice), renderPass(renderPass),
      swapChain(swapChain), pipeline(pipeline) {
  createCommandPool(logicalDevice);
  createCommandBuffers(logicalDevice);
}

CommandPool::~CommandPool() {
  vkDestroyCommandPool(logicalDevice.data(), commandPool, nullptr);
}

void CommandPool::createCommandPool(const LogicalDevice &logicalDevice) {
  auto queueFamilyIndices =
      logicalDevice.getPhysicalDevice().getQueueFamilyIndices();

  VkCommandPoolCreateInfo poolInfo{};
  poolInfo.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
  poolInfo.flags = VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT;
  poolInfo.queueFamilyIndex = queueFamilyIndices.graphicsFamily.value();

  if (vkCreateCommandPool(logicalDevice.data(), &poolInfo, nullptr,
                          &commandPool) != VK_SUCCESS) {
    throw std::runtime_error("failed to create command pool!");
  }
}

void CommandPool::createCommandBuffers(const LogicalDevice &logicalDevice) {
  commandBuffers.resize(MAX_FRAMES_IN_FLIGHT);

  VkCommandBufferAllocateInfo allocInfo{};
  allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
  allocInfo.commandPool = commandPool;
  allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
  allocInfo.commandBufferCount = static_cast<uint32_t>(commandBuffers.size());

  if (vkAllocateCommandBuffers(logicalDevice.data(), &allocInfo,
                               commandBuffers.data()) != VK_SUCCESS) {
    throw std::runtime_error("failed to allocate command buffers!");
  }
}

void CommandPool::resetCommandBuffer(uint32_t commandBufferIndex) {
  vkResetCommandBuffer(commandBuffers[commandBufferIndex], 0);
}

void CommandPool::recordCommandBuffer(uint32_t commandBufferIndex,
                                      uint32_t imageIndex) {
  auto commandBuffer = commandBuffers[commandBufferIndex];

  VkCommandBufferBeginInfo beginInfo{};
  beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
  beginInfo.flags = 0;                  // Optional
  beginInfo.pInheritanceInfo = nullptr; // Optional

  if (vkBeginCommandBuffer(commandBuffer, &beginInfo) != VK_SUCCESS) {
    throw std::runtime_error("failed to begin recording command buffer!");
  }

  auto extent = swapChain.getExtent();

  VkClearValue clearColor = {{{0.0f, 0.0f, 0.0f, 1.0f}}};
  VkRenderPassBeginInfo renderPassInfo{};
  renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
  renderPassInfo.renderPass = renderPass.data();
  renderPassInfo.framebuffer = swapChain.getFramebuffer(imageIndex);
  renderPassInfo.renderArea.offset = {0, 0};
  renderPassInfo.renderArea.extent = extent;
  renderPassInfo.clearValueCount = 1;
  renderPassInfo.pClearValues = &clearColor;

  vkCmdBeginRenderPass(commandBuffer, &renderPassInfo,
                       VK_SUBPASS_CONTENTS_INLINE);
  vkCmdBindPipeline(commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS,
                    pipeline.data());

  VkViewport viewport{};
  viewport.x = 0.0f;
  viewport.y = 0.0f;
  viewport.width = static_cast<float>(extent.width);
  viewport.height = static_cast<float>(extent.height);
  viewport.minDepth = 0.0f;
  viewport.maxDepth = 1.0f;
  vkCmdSetViewport(commandBuffer, 0, 1, &viewport);

  VkRect2D scissor{};
  scissor.offset = {0, 0};
  scissor.extent = extent;
  vkCmdSetScissor(commandBuffer, 0, 1, &scissor);

  vkCmdDraw(commandBuffer, 3, 1, 0, 0);

  vkCmdEndRenderPass(commandBuffer);
  if (vkEndCommandBuffer(commandBuffer) != VK_SUCCESS) {
    throw std::runtime_error("failed to record command buffer!");
  }
}

} // namespace engine::vulkan
