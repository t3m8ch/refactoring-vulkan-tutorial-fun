#pragma once

#include <iostream>

#include <vulkan/vulkan_core.h>

#define RESET "\033[0m"
#define RED "\033[31m"
#define GREEN "\033[32m"
#define YELLOW "\033[33m"

static VKAPI_ATTR VkBool32 VKAPI_CALL
debugCallback(VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity,
              VkDebugUtilsMessageTypeFlagsEXT messageType,
              const VkDebugUtilsMessengerCallbackDataEXT *pCallbackData,
              void *pUserData) {
  std::string severityStr;
  const char *color = RESET;

  if (messageSeverity & VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT) {
    severityStr = "VERBOSE";
    color = GREEN;
  } else if (messageSeverity & VK_DEBUG_UTILS_MESSAGE_SEVERITY_INFO_BIT_EXT) {
    severityStr = "INFO";
    color = GREEN;
  } else if (messageSeverity &
             VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT) {
    severityStr = "WARNING";
    color = YELLOW;
  } else if (messageSeverity & VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT) {
    severityStr = "ERROR";
    color = RED;
  } else {
    severityStr = "UNKNOWN";
    color = RESET;
  }

  std::cerr << "VALIDATION LAYER" << color << " [" << severityStr << "]:\t"
            << RESET << pCallbackData->pMessage << std::endl;

  return VK_FALSE;
}
