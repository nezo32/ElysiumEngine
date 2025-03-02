#include "swap_chain.hpp"

#include <algorithm>
#include <limits>

#include "dependencies.hpp"
#include "utils/queue_families.hpp"


namespace Ely {

SwapChain::SwapChain(ElysiumDependencies &deps) : deps{deps} {
    querySwapChainSupport();
    chooseSwapSurfaceFormat();
    chooseSwapPresentMode();
    chooseSwapExtent();
    createSwapChain();
    createSwapChainImages();
    createSwapChainImageViews();
}

SwapChain::~SwapChain() {
    auto device = deps.device->GetDevice();

    for (auto imageView : swapChainImageViews) {
        vkDestroyImageView(device, imageView, nullptr);
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

SwapChainNextImageResult SwapChain::NextImage(VkSemaphore semaphore, uint64_t timeout, VkFence fence) {
    SwapChainNextImageResult ret{};
    VkResult result = vkAcquireNextImageKHR(deps.device->GetDevice(), swapChain, timeout, semaphore, fence, &ret.image);
    ret.result = result;
    return ret;
}

void SwapChain::querySwapChainSupport() {
    auto device = deps.physDevice->GetPhysicalDevice();
    auto surface = deps.vulkan->GetSurface();

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

#ifdef _DEBUG
            std::cout << "Present mode: Mailbox" << std::endl;
#endif

            return;
        }
    }

    presentMode = VK_PRESENT_MODE_FIFO_KHR;
#ifdef _DEBUG
    std::cout << "Present mode: FIFO" << std::endl;
#endif
}

void SwapChain::chooseSwapExtent() {
    if (details.capabilities.currentExtent.width != std::numeric_limits<uint32_t>::max()) {
        extent = details.capabilities.currentExtent;
        return;
    } else {
        VkExtent2D actualExtent = deps.window->GetExtent();

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
    createInfo.surface = deps.vulkan->GetSurface();
    createInfo.minImageCount = imageCount;
    createInfo.imageFormat = surfaceFormat.format;
    createInfo.imageColorSpace = surfaceFormat.colorSpace;
    createInfo.imageExtent = extent;
    createInfo.imageArrayLayers = 1;
    createInfo.imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;

    auto indices = findQueueFamilies(deps.vulkan->GetSurface(), deps.physDevice->GetPhysicalDevice());
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

    if (vkCreateSwapchainKHR(deps.device->GetDevice(), &createInfo, nullptr, &swapChain) != VK_SUCCESS) {
        throw std::runtime_error("Failed to create swap chain");
    }
}

void SwapChain::createSwapChainImages() {
    uint32_t imageCount;
    vkGetSwapchainImagesKHR(deps.device->GetDevice(), swapChain, &imageCount, nullptr);
    swapChainImages.resize(imageCount);
    vkGetSwapchainImagesKHR(deps.device->GetDevice(), swapChain, &imageCount, swapChainImages.data());
    swapChainImageFormat = surfaceFormat.format;
}

void SwapChain::createSwapChainImageViews() {
    swapChainImageViews.resize(swapChainImages.size());

    for (size_t i = 0; i < swapChainImages.size(); i++) {
        VkImageViewCreateInfo createInfo{};
        createInfo.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
        createInfo.image = swapChainImages[i];
        createInfo.viewType = VK_IMAGE_VIEW_TYPE_2D;
        createInfo.format = swapChainImageFormat;

        createInfo.components.r = VK_COMPONENT_SWIZZLE_IDENTITY;
        createInfo.components.g = VK_COMPONENT_SWIZZLE_IDENTITY;
        createInfo.components.b = VK_COMPONENT_SWIZZLE_IDENTITY;
        createInfo.components.a = VK_COMPONENT_SWIZZLE_IDENTITY;

        createInfo.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
        createInfo.subresourceRange.baseMipLevel = 0;
        createInfo.subresourceRange.levelCount = 1;
        createInfo.subresourceRange.baseArrayLayer = 0;
        createInfo.subresourceRange.layerCount = 1;

        if (vkCreateImageView(deps.device->GetDevice(), &createInfo, nullptr, &swapChainImageViews[i]) != VK_SUCCESS) {
            throw std::runtime_error("failed to create image views!");
        }
    }
}

}   // namespace Ely