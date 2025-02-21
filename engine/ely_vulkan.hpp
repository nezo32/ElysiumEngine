#pragma once

#include <stdexcept>
#include <vector>

#include "external/ely_glfw.hpp"

namespace Ely {
class ElyVulkan {
   private:
#ifndef _DEBUG
    const bool enableValidationLayers = false;
#else
    const bool enableValidationLayers = true;
#endif
    const std::vector<const char *> validationLayers = {"VK_LAYER_KHRONOS_validation"};
    const std::vector<const char *> deviceExtensions = {VK_KHR_SWAPCHAIN_EXTENSION_NAME};

    VkInstance instance;
    VkDebugUtilsMessengerEXT debugMessenger;

    bool checkValidationLayerSupport();

    void createVulkanInstance(const char *appName, uint32_t appVersion, VkDebugUtilsMessengerCreateInfoEXT *debugInfo);
    VkApplicationInfo createApplicationInfo(const char *appName, uint32_t appVersion);
    VkDebugUtilsMessengerCreateInfoEXT createDebugMessengerInfo();
    void createDebugUtilsMessenger(VkDebugUtilsMessengerCreateInfoEXT *createInfo);

    std::vector<const char *> getRequiredInstanceExtensions();

    // DEBUG LAYERS
    static VKAPI_ATTR VkBool32 VKAPI_CALL debugCallback(VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity,
                                                        VkDebugUtilsMessageTypeFlagsEXT messageType,
                                                        const VkDebugUtilsMessengerCallbackDataEXT *pCallbackData,
                                                        void *pUserData);
    static void DestroyDebugUtilsMessengerEXT(VkInstance instance, VkDebugUtilsMessengerEXT debugMessenger,
                                              const VkAllocationCallbacks *pAllocator);
    static VkResult CreateDebugUtilsMessengerEXT(VkInstance instance,
                                                 const VkDebugUtilsMessengerCreateInfoEXT *pCreateInfo,
                                                 const VkAllocationCallbacks *pAllocator,
                                                 VkDebugUtilsMessengerEXT *pDebugMessenger);

   public:
    ElyVulkan(const char *appName, uint32_t appVersion);
    ~ElyVulkan();

    ElyVulkan(const ElyVulkan &) = delete;
    void operator=(const ElyVulkan &) = delete;
    ElyVulkan(ElyVulkan &&) = delete;
    ElyVulkan &operator=(ElyVulkan &&) = delete;

    VkInstance GetInstance() { return instance; }

    static uint32_t GetVersionCode(uint32_t major, uint32_t minor, uint32_t patch) {
        return VK_MAKE_VERSION(major, minor, patch);
    }
};
}   // namespace Ely