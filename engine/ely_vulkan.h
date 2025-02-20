#pragma once

#include "external/ely_glfw.h"
#include <stdexcept>

namespace Ely {
class ElyVulkan {
private:
  VkInstance instance;
  VkInstanceCreateInfo createInfo{};
  VkApplicationInfo appInfo{};

  static inline const char *ENGINE_NAME = "Elysium";

  uint32_t glfwExtensionCount = 0;
  const char **glfwExtensions = nullptr;

  void configAppInfo(const char *appName);
  void configCreateInfo();
  void configGLFWExtensions();

public:
  ElyVulkan(const char *appName);
  ~ElyVulkan();

  ElyVulkan(const ElyVulkan &) = delete;
  ElyVulkan &operator=(const ElyVulkan &) = delete;
};
} // namespace Ely