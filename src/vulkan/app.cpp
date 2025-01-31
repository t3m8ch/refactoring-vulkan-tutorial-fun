#include "app.hpp"

namespace engine::vulkan {
VulkanApp::VulkanApp(const Config &config) : config(config) {}

void VulkanApp::run() {
  while (!glfwWindowShouldClose(window.data())) {
    glfwPollEvents();
    drawFrame();
  }

  logicalDevice.waitIdle();
}

void VulkanApp::drawFrame() {
  uint32_t frame = swapChain.getCurrentFrame();
  auto imageAvailableSemaphore = sync.getImageAvailableSemaphores()[frame];
  auto renderFinishedSemaphore = sync.getRenderFinishedSemaphores()[frame];
  auto inFlightFence = sync.getInFlightFences()[frame];

  sync.waitForInFlightFence(frame);

  auto imageIndex = swapChain.acquireNextImage(imageAvailableSemaphore);
  if (!imageIndex.has_value()) {
    return;
  }

  sync.resetInFilghtFence(frame);

  commandPool.resetCommandBuffer(frame);
  commandPool.recordCommandBuffer(frame, imageIndex.value());

  logicalDevice.graphicsQueueSubmit(commandPool.getCommandBuffer(frame),
                                    imageAvailableSemaphore,
                                    renderFinishedSemaphore, inFlightFence);

  swapChain.present(imageIndex.value(), renderFinishedSemaphore);
  swapChain.nextFrame();
}

} // namespace engine::vulkan
