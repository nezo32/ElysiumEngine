#include "ely_swap_chain.hpp"

#include <algorithm>
#include <limits>

#include "ely_queue_families.hpp"

namespace Ely {
SwapChain::SwapChain(Window &window, Vulkan &vulkan, PhysDevice &physDevice, Device &device)
    : elyWindow{window}, elyVulkan{vulkan}, elyPhysDevice{physDevice}, elyDevice{device} {
    querySwapChainSupport();
    chooseSwapSurfaceFormat();
    chooseSwapPresentMode();
    chooseSwapExtent();
    createSwapChain();

    uint32_t imageCount;
    vkGetSwapchainImagesKHR(device.GetDevice(), swapChain, &imageCount, nullptr);
    swapChainImages.resize(imageCount);
    vkGetSwapchainImagesKHR(device.GetDevice(), swapChain, &imageCount, swapChainImages.data());
}

SwapChain::~SwapChain() {
    auto device = elyDevice.GetDevice();

    for (auto &image : swapChainImages) {
        vkDestroyImage(device, image, nullptr);
    }

    vkDestroySwapchainKHR(device, swapChain, nullptr);
}

SwapChainSupportDetails SwapChain::QuerySwapChainSupport(VkSurfaceKHR surface, VkPhysicalDevice device) {
    SwapChainSupportDetails details{};

    vkGetPhysicalDeviceSurfaceCapabilitiesKHR(device, surface, &details.capabilities);

    uint32_t formatCount;
    vkGetPhysicalDeviceSurfaceFormatsKHR(device, surface, &formatCount, nullptr);
    if (formatCount != 0) {
        details.formats.resize(formatCount);
        vkGetPhysicalDeviceSurfaceFormatsKHR(device, surface, &formatCount, details.formats.data());
    }

    uint32_t presentModeCount;
    vkGetPhysicalDeviceSurfacePresentModesKHR(device, surface, &presentModeCount, nullptr);
    if (presentModeCount != 0) {
        details.presentModes.resize(presentModeCount);
        vkGetPhysicalDeviceSurfacePresentModesKHR(device, surface, &presentModeCount, details.presentModes.data());
    }

    return details;
}

void SwapChain::querySwapChainSupport() {
    auto device = elyPhysDevice.GetPhysicalDevice();
    auto surface = elyVulkan.GetSurface();

    details = QuerySwapChainSupport(surface, device);
}

void SwapChain::chooseSwapSurfaceFormat() {
    for (const auto &availableFormat : details.formats) {
        if (availableFormat.format == VK_FORMAT_B8G8R8A8_SRGB &&
            availableFormat.colorSpace == VK_COLOR_SPACE_SRGB_NONLINEAR_KHR) {
            surfaceFormat = availableFormat;
            return;
        }
    }

    surfaceFormat = details.formats[0];
}

void SwapChain::chooseSwapPresentMode() {
    for (const auto &availablePresentMode : details.presentModes) {
        if (availablePresentMode == VK_PRESENT_MODE_MAILBOX_KHR) {
            presentMode = availablePresentMode;
            return;
        }
    }

    presentMode = VK_PRESENT_MODE_FIFO_KHR;
}

void SwapChain::chooseSwapExtent() {
    if (details.capabilities.currentExtent.width != std::numeric_limits<uint32_t>::max()) {
        extent = details.capabilities.currentExtent;
        return;
    } else {
        int width, height;
        glfwGetFramebufferSize(elyWindow.GetWindow(), &width, &height);

        VkExtent2D actualExtent = {static_cast<uint32_t>(width), static_cast<uint32_t>(height)};

        actualExtent.width = std::clamp(actualExtent.width, details.capabilities.minImageExtent.width,
                                        details.capabilities.maxImageExtent.width);
        actualExtent.height = std::clamp(actualExtent.height, details.capabilities.minImageExtent.height,
                                         details.capabilities.maxImageExtent.height);

        extent = actualExtent;
    }
}

void SwapChain::createSwapChain() {
    uint32_t imageCount = details.capabilities.minImageCount + 1;
    uint32_t maxImageCount = details.capabilities.maxImageCount;
    if (maxImageCount > 0 && imageCount > maxImageCount) {
        imageCount = maxImageCount;
    }

    VkSwapchainCreateInfoKHR createInfo{};
    createInfo.sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
    createInfo.surface = elyVulkan.GetSurface();
    createInfo.minImageCount = imageCount;
    createInfo.imageFormat = surfaceFormat.format;
    createInfo.imageColorSpace = surfaceFormat.colorSpace;
    createInfo.imageExtent = extent;
    createInfo.imageArrayLayers = 1;
    createInfo.imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;

    QueueFamilyIndices indices =
        QueueFamilies::FindQueueFamilies(elyVulkan.GetSurface(), elyPhysDevice.GetPhysicalDevice());
    uint32_t queueFamilyIndices[] = {indices.graphicsFamily.value(), indices.presentFamily.value()};

    if (indices.graphicsFamily != indices.presentFamily) {
        createInfo.imageSharingMode = VK_SHARING_MODE_CONCURRENT;
        createInfo.queueFamilyIndexCount = 2;
        createInfo.pQueueFamilyIndices = queueFamilyIndices;
    } else {
        createInfo.imageSharingMode = VK_SHARING_MODE_EXCLUSIVE;
        createInfo.queueFamilyIndexCount = 0;       // Optional
        createInfo.pQueueFamilyIndices = nullptr;   // Optional
    }

    createInfo.preTransform = details.capabilities.currentTransform;
    createInfo.compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;
    createInfo.presentMode = presentMode;
    createInfo.clipped = VK_TRUE;
    createInfo.oldSwapchain = VK_NULL_HANDLE;

    if (vkCreateSwapchainKHR(elyDevice.GetDevice(), &createInfo, nullptr, &swapChain) != VK_SUCCESS) {
        throw std::runtime_error("Failed to create swap chain");
    }
}

void SwapChain::createSwapChainImages() {}

void SwapChain::createSwapChainImageViews() {}

}   // namespace Ely