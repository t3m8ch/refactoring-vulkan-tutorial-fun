#include "sync.hpp"
#include "constants.hpp"

#include <vulkan/vulkan_core.h>

namespace engine::vulkan {

Sync::Sync(const LogicalDevice &logicalDevice) : logicalDevice(logicalDevice) {
  imageAvailableSemaphores.resize(MAX_FRAMES_IN_FLIGHT);
  renderFinishedSemaphores.resize(MAX_FRAMES_IN_FLIGHT);
  inFlightFences.resize(MAX_FRAMES_IN_FLIGHT);

  VkSemaphoreCreateInfo semaphoreInfo{};
  semaphoreInfo.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;

  VkFenceCreateInfo fenceInfo{};
  fenceInfo.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;
  fenceInfo.flags = VK_FENCE_CREATE_SIGNALED_BIT;

  for (size_t i = 0; i < MAX_FRAMES_IN_FLIGHT; i++) {
    auto imageAvailableSemaphoresResult =
        vkCreateSemaphore(logicalDevice.data(), &semaphoreInfo, nullptr,
                          &imageAvailableSemaphores[i]);
    auto renderFinishedSemaphoresResult =
        vkCreateSemaphore(logicalDevice.data(), &semaphoreInfo, nullptr,
                          &renderFinishedSemaphores[i]);
    auto inFlightFencesResult = vkCreateFence(logicalDevice.data(), &fenceInfo,
                                              nullptr, &inFlightFences[i]);

    if (imageAvailableSemaphoresResult != VK_SUCCESS ||
        renderFinishedSemaphoresResult != VK_SUCCESS ||
        inFlightFencesResult != VK_SUCCESS) {
      throw std::runtime_error("Failed to create synchronization objects for "
                               "a frame.");
    }
  }
}

Sync::~Sync() {
  for (size_t i = 0; i < MAX_FRAMES_IN_FLIGHT; i++) {
    vkDestroySemaphore(logicalDevice.data(), renderFinishedSemaphores[i],
                       nullptr);
    vkDestroySemaphore(logicalDevice.data(), imageAvailableSemaphores[i],
                       nullptr);
    vkDestroyFence(logicalDevice.data(), inFlightFences[i], nullptr);
  }
}

void Sync::waitForInFlightFence(uint32_t fenceIndex) {
  vkWaitForFences(logicalDevice.data(), 1, &inFlightFences[fenceIndex], VK_TRUE,
                  UINT64_MAX);
}

void Sync::resetInFilghtFence(uint32_t fenceIndex) {
  vkResetFences(logicalDevice.data(), 1, &inFlightFences[fenceIndex]);
}

} // namespace engine::vulkan
