#pragma once

#include <vector>

#include "device/device.hpp"
#include "device/phys_device.hpp"
#include "external/glfw.hpp"
#include "vulkan.hpp"
#include "window.hpp"

namespace Ely {
struct SwapChainSupportDetails {
    VkSurfaceCapabilitiesKHR capabilities;
    std::vector<VkSurfaceFormatKHR> formats;
    std::vector<VkPresentModeKHR> presentModes;
};

struct SwapChainNextImageResult {
    uint32_t image;
    VkResult result;
};

class PhysDevice;
class Device;
class SwapChain {
   private:
    VkSwapchainKHR swapChain;

    Window &elyWindow;
    Vulkan &elyVulkan;
    PhysDevice &elyPhysDevice;
    Device &elyDevice;

    SwapChainSupportDetails details;
    VkSurfaceFormatKHR surfaceFormat{};
    VkPresentModeKHR presentMode;
    VkExtent2D extent{};
    VkFormat swapChainImageFormat;
    std::vector<VkImage> swapChainImages;
    std::vector<VkImageView> swapChainImageViews;

    void querySwapChainSupport();
    void chooseSwapSurfaceFormat();
    void chooseSwapPresentMode();
    void chooseSwapExtent();
    void createSwapChain();
    void createSwapChainImages();
    void createSwapChainImageViews();

   public:
    SwapChain(Window &window, Vulkan &vulkan, PhysDevice &physDevice, Device &device);
    ~SwapChain();

    SwapChain(const SwapChain &) = delete;
    SwapChain &operator=(const SwapChain &) = delete;

    VkSwapchainKHR GetSwapChain() { return swapChain; }
    VkFormat GetImageFormat() { return swapChainImageFormat; }
    std::vector<VkImageView> GetImageViews() { return swapChainImageViews; }
    VkExtent2D GetExtent() { return extent; }

    SwapChainNextImageResult NextImage(VkSemaphore semaphore = nullptr, uint64_t timeout = UINT64_MAX,
                                       VkFence fence = VK_NULL_HANDLE);

    static SwapChainSupportDetails QuerySwapChainSupport(VkSurfaceKHR surface, VkPhysicalDevice device);
};
}   // namespace Ely