#include "ely_phys_device.hpp"

#include <map>

#include "ely_queue_families.hpp"

namespace Ely {

ElyPhysDevice::ElyPhysDevice(ElyVulkan& vulkan) : elyVulkan{vulkan} { pickPhysicalDevice(); }

ElyPhysDevice::~ElyPhysDevice() {}

void ElyPhysDevice::pickPhysicalDevice() {
    uint32_t deviceCount = 0;
    vkEnumeratePhysicalDevices(elyVulkan.GetInstance(), &deviceCount, nullptr);
    if (deviceCount == 0) {
        throw std::runtime_error("Failed to find GPUs with Vulkan support");
    }

    std::vector<VkPhysicalDevice> devices(deviceCount);
    vkEnumeratePhysicalDevices(elyVulkan.GetInstance(), &deviceCount, devices.data());

    std::multimap<int, VkPhysicalDevice> candidates;
    for (const auto& device : devices) {
        int score = rateDeviceSuitability(device);
        candidates.insert(std::make_pair(score, device));
    }

    // Check if the best candidate is suitable at all
    if (candidates.rbegin()->first > 0) {
        physicalDevice = candidates.rbegin()->second;
    } else {
        throw std::runtime_error("Failed to find a suitable GPU");
    }
}

int ElyPhysDevice::rateDeviceSuitability(VkPhysicalDevice device) {
    VkPhysicalDeviceProperties deviceProperties{};
    VkPhysicalDeviceFeatures deviceFeatures{};
    vkGetPhysicalDeviceProperties(device, &deviceProperties);
    vkGetPhysicalDeviceFeatures(device, &deviceFeatures);

    int score = 0;
    if (deviceProperties.deviceType == VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU) {
        score += 1000;
    }

    score += deviceProperties.limits.maxImageDimension2D;
    score += deviceProperties.limits.maxPushConstantsSize;

    if (!deviceFeatures.geometryShader || !ElyQueueFamilies::FindQueueFamilies(device).isComplete()) {
        return 0;
    }

    return score;
}

}   // namespace Ely