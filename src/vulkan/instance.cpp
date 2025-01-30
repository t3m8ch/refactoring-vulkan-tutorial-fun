#define GLFW_INCLUDE_VULKAN

#include "instance.hpp"
#include "constants.hpp"
#include "debugCallback.hpp"

#include <cstring>
#include <stdexcept>
#include <vector>

#include <GLFW/glfw3.h>
#include <vulkan/vulkan_core.h>

namespace engine::vulkan {

Instance::Instance(const Config &config) : config(config) {
  if (config.enableValidationLayers && !checkValidationLayerSupport()) {
    throw std::runtime_error("Validation layers requested but not available");
  }

  VkInstanceCreateInfo instanceCreateInfo{};
  instanceCreateInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;

  auto appInfo = getAppInfo();
  instanceCreateInfo.pApplicationInfo = &appInfo;

  auto extensions = getExtensions();
  instanceCreateInfo.enabledExtensionCount = extensions.size();
  instanceCreateInfo.ppEnabledExtensionNames = extensions.data();

  auto debugMessengerCreateInfo = getDebugMessengerCreateInfo();

  if (config.enableValidationLayers) {
    instanceCreateInfo.enabledLayerCount =
        static_cast<uint32_t>(validationLayers.size());
    instanceCreateInfo.ppEnabledLayerNames = validationLayers.data();
    instanceCreateInfo.pNext = &debugMessengerCreateInfo;
  } else {
    instanceCreateInfo.enabledLayerCount = 0;
    instanceCreateInfo.pNext = nullptr;
  }

  if (vkCreateInstance(&instanceCreateInfo, nullptr, &instance) != VK_SUCCESS) {
    throw std::runtime_error("Failed to create instance");
  }
}

Instance::~Instance() { vkDestroyInstance(instance, nullptr); }

VkApplicationInfo Instance::getAppInfo() const {
  VkApplicationInfo appInfo{};

  appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
  appInfo.pApplicationName = config.appName.data();
  appInfo.applicationVersion = config.appVersion.vkMakeVersion();
  appInfo.pEngineName = config.engineName.data();
  appInfo.engineVersion = config.engineVersion.vkMakeVersion();
  appInfo.apiVersion = VK_API_VERSION_1_3;

  return appInfo;
}

std::vector<const char *> Instance::getExtensions() const {
  uint32_t glfwExtensionCount = 0;
  const char **glfwExtensions;

  glfwExtensions = glfwGetRequiredInstanceExtensions(&glfwExtensionCount);

  std::vector<const char *> extensions(glfwExtensions,
                                       glfwExtensions + glfwExtensionCount);

  extensions.emplace_back(VK_KHR_PORTABILITY_ENUMERATION_EXTENSION_NAME);

  if (config.enableValidationLayers) {
    extensions.emplace_back(VK_EXT_DEBUG_UTILS_EXTENSION_NAME);
  }

  return extensions;
}

VkDebugUtilsMessengerCreateInfoEXT Instance::getDebugMessengerCreateInfo() {
  VkDebugUtilsMessengerCreateInfoEXT messengerCreateInfo;
  messengerCreateInfo.sType =
      VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT;
  messengerCreateInfo.messageSeverity =
      VK_DEBUG_UTILS_MESSAGE_SEVERITY_INFO_BIT_EXT |
      VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT |
      VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT |
      VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT;
  messengerCreateInfo.messageType =
      VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT |
      VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT |
      VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT;
  messengerCreateInfo.pfnUserCallback = debugCallback;

  return messengerCreateInfo;
}

bool Instance::checkValidationLayerSupport() const {
  uint32_t layerCount;
  vkEnumerateInstanceLayerProperties(&layerCount, nullptr);

  std::vector<VkLayerProperties> availableLayers(layerCount);
  vkEnumerateInstanceLayerProperties(&layerCount, availableLayers.data());

  for (auto layerName : validationLayers) {
    bool layerFound = false;

    for (const auto &layerProperties : availableLayers) {
      if (strcmp(layerName, layerProperties.layerName)) {
        layerFound = true;
        break;
      }
    }

    if (!layerFound) {
      return false;
    }
  }

  return true;
}

} // namespace engine::vulkan
