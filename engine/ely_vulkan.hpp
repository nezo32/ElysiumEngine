#pragma once

#include <stdexcept>
#include <vector>

#include "ely_window.hpp"
#include "external/ely_glfw.hpp"

namespace Ely {
static VKAPI_ATTR VkBool32 VKAPI_CALL debugCallback(VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity,
                                                    VkDebugUtilsMessageTypeFlagsEXT messageType,
                                                    const VkDebugUtilsMessengerCallbackDataEXT *pCallbackData,
                                                    void *pUserData);
static void DestroyDebugUtilsMessengerEXT(VkInstance instance, VkDebugUtilsMessengerEXT debugMessenger,
                                          const VkAllocationCallbacks *pAllocator);
static VkResult CreateDebugUtilsMessengerEXT(VkInstance instance, const VkDebugUtilsMessengerCreateInfoEXT *pCreateInfo,
                                             const VkAllocationCallbacks *pAllocator,
                                             VkDebugUtilsMessengerEXT *pDebugMessenger);
class Vulkan {
   private:
#ifndef _DEBUG
    const bool enableValidationLayers = false;
#else
    const bool enableValidationLayers = true;
#endif
    const std::vector<const char *> validationLayers = {"VK_LAYER_KHRONOS_validation"};
    const std::vector<const char *> deviceExtensions = {VK_KHR_SWAPCHAIN_EXTENSION_NAME};

    VkInstance instance;
    VkSurfaceKHR surface;
    VkDebugUtilsMessengerEXT debugMessenger;

    bool checkValidationLayerSupport();

    void createVulkanInstance(const char *appName, uint32_t appVersion, VkDebugUtilsMessengerCreateInfoEXT *debugInfo);
    VkApplicationInfo createApplicationInfo(const char *appName, uint32_t appVersion);
    VkDebugUtilsMessengerCreateInfoEXT createDebugMessengerInfo();
    void createDebugUtilsMessenger(VkDebugUtilsMessengerCreateInfoEXT *createInfo);

    std::vector<const char *> getRequiredInstanceExtensions();
    void debugExtensions();

   public:
    Vulkan(Window &elyWindow, const char *appName, uint32_t appVersion);
    ~Vulkan();

    Vulkan(const Vulkan &) = delete;
    void operator=(const Vulkan &) = delete;
    Vulkan(Vulkan &&) = delete;
    Vulkan &operator=(Vulkan &&) = delete;

    VkInstance GetInstance() { return instance; }
    VkSurfaceKHR GetSurface() { return surface; }
    const std::vector<const char *> GetExtensions() { return deviceExtensions; }
};
}   // namespace Ely