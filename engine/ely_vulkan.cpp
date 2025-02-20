#include "ely_vulkan.h"

namespace Ely {

ElyVulkan::ElyVulkan(const char *appName) {
  configAppInfo(appName);
  configGLFWExtensions();
  configCreateInfo();

  if (vkCreateInstance(&createInfo, nullptr, &instance) != VK_SUCCESS) {
    throw std::exception("ERROR :: ElyVulkan :: Failed to create instance");
  }
}

ElyVulkan::~ElyVulkan() { vkDestroyInstance(instance, nullptr); }

void ElyVulkan::configAppInfo(const char *appName) {
  appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;

  appInfo.pApplicationName = appName;
  appInfo.applicationVersion = VK_MAKE_VERSION(1, 0, 0);

  appInfo.pEngineName = ENGINE_NAME;
  appInfo.engineVersion = VK_MAKE_VERSION(1, 0, 0);

  appInfo.apiVersion = VK_API_VERSION_1_0;
}

void ElyVulkan::configCreateInfo() {
  createInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
  createInfo.pApplicationInfo = &appInfo;

  createInfo.enabledExtensionCount = glfwExtensionCount;
  createInfo.ppEnabledExtensionNames = glfwExtensions;

  createInfo.enabledLayerCount = 0;
}

void ElyVulkan::configGLFWExtensions() {
  glfwExtensions = glfwGetRequiredInstanceExtensions(&glfwExtensionCount);
}

} // namespace Ely
