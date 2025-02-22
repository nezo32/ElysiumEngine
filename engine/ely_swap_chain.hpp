#pragma once

#include <vector>

#include "ely_device.hpp"
#include "ely_phys_device.hpp"
#include "ely_vulkan.hpp"
#include "ely_window.hpp"
#include "external/ely_glfw.hpp"

namespace Ely {
struct SwapChainSupportDetails {
    VkSurfaceCapabilitiesKHR capabilities;
    std::vector<VkSurfaceFormatKHR> formats;
    std::vector<VkPresentModeKHR> presentModes;
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

    static SwapChainSupportDetails QuerySwapChainSupport(VkSurfaceKHR surface, VkPhysicalDevice device);
};
}   // namespace Ely