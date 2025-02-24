#include "phys_device.hpp"

#include <iostream>
#include <map>
#include <set>

#include "swap_chain.hpp"
#include "utils/queue_families.hpp"


namespace Ely {

PhysDevice::PhysDevice(Vulkan& vulkan) : elyVulkan{vulkan} { pickPhysicalDevice(); }

PhysDevice::~PhysDevice() {}

void PhysDevice::pickPhysicalDevice() {
    uint32_t deviceCount = 0;
    vkEnumeratePhysicalDevices(elyVulkan.GetInstance(), &deviceCount, nullptr);
    if (deviceCount == 0) {
        throw std::runtime_error("Failed to find GPUs with Vulkan support");
    }

    std::vector<VkPhysicalDevice> devices(deviceCount);
    vkEnumeratePhysicalDevices(elyVulkan.GetInstance(), &deviceCount, devices.data());

    std::multimap<uint32_t, VkPhysicalDevice> candidates;
    for (const auto& device : devices) {
        uint32_t score = rateDeviceSuitability(device);
        candidates.insert(std::make_pair(score, device));
    }

#ifdef _DEBUG

    std::cout << std::endl << "devices scoring:" << std::endl;
    for (auto it = candidates.begin(); it != candidates.end(); it++) {
        VkPhysicalDeviceProperties properties;
        vkGetPhysicalDeviceProperties(it->second, &properties);
        std::cout << "\t" << properties.deviceName << " - " << it->first << std::endl;
    }
#endif

    // Check if the best candidate is suitable at all
    if (candidates.rbegin()->first > 0) {
        physicalDevice = candidates.rbegin()->second;
    } else {
        throw std::runtime_error("Failed to find a suitable GPU");
    }

#ifdef _DEBUG
    VkPhysicalDeviceProperties properties;
    vkGetPhysicalDeviceProperties(physicalDevice, &properties);
    std::cout << "selected device: " << std::endl << "\t" << properties.deviceName << std::endl;
#endif
}

uint32_t PhysDevice::rateDeviceSuitability(VkPhysicalDevice device) {
    VkPhysicalDeviceProperties deviceProperties{};
    VkPhysicalDeviceFeatures deviceFeatures{};
    vkGetPhysicalDeviceProperties(device, &deviceProperties);
    vkGetPhysicalDeviceFeatures(device, &deviceFeatures);

    bool extensionsSupported = checkDeviceExtensionSupport(device);
    bool queueFamiliesSupported = QueueFamilies::FindQueueFamilies(elyVulkan.GetSurface(), device).isComplete();
    bool swapChainAdequate = false;

    if (extensionsSupported) {
        auto swapChainSupport = Ely::SwapChain::QuerySwapChainSupport(elyVulkan.GetSurface(), device);
        swapChainAdequate = !swapChainSupport.formats.empty() && !swapChainSupport.presentModes.empty();
    }

    if (!deviceFeatures.geometryShader || !extensionsSupported || !swapChainAdequate || !queueFamiliesSupported) {
        return 0;
    }

    uint32_t score = 0;
    if (deviceProperties.deviceType == VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU) {
        score += 1000;
    }

    score += deviceProperties.limits.maxImageDimension2D;
    score += deviceProperties.limits.maxPushConstantsSize;

    return score;
}

bool PhysDevice::checkDeviceExtensionSupport(VkPhysicalDevice device) {
    uint32_t extensionCount;
    vkEnumerateDeviceExtensionProperties(device, nullptr, &extensionCount, nullptr);

    std::vector<VkExtensionProperties> availableExtensions(extensionCount);
    vkEnumerateDeviceExtensionProperties(device, nullptr, &extensionCount, availableExtensions.data());

    auto deviceExtensions = elyVulkan.GetExtensions();
    std::set<std::string> requiredExtensions(deviceExtensions.begin(), deviceExtensions.end());

    for (const auto& extension : availableExtensions) {
        requiredExtensions.erase(extension.extensionName);
    }

    return requiredExtensions.empty();
}

}   // namespace Ely