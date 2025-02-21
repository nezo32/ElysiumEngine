#include "ely_logic_device.hpp"

#include "ely_queue_families.hpp"

namespace Ely {

ElyLogicDevice::ElyLogicDevice(ElyPhysDevice &physDevice) : elyPhysDevice(physDevice) {
    QueueFamilyIndices indices = ElyQueueFamilies::FindQueueFamilies(elyPhysDevice.GetPhysicalDevice());

    float queuePriority = 1.0f;

    VkDeviceQueueCreateInfo queueCreateInfo{};
    queueCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
    queueCreateInfo.queueFamilyIndex = indices.graphicsFamily.value();
    queueCreateInfo.queueCount = 1;
    queueCreateInfo.pQueuePriorities = &queuePriority;

    VkPhysicalDeviceFeatures deviceFeatures{};

    VkDeviceCreateInfo createInfo{};
    createInfo.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
    createInfo.pQueueCreateInfos = &queueCreateInfo;
    createInfo.queueCreateInfoCount = 1;
    createInfo.pEnabledFeatures = &deviceFeatures;
    createInfo.enabledExtensionCount = 0;

    if (vkCreateDevice(elyPhysDevice.GetPhysicalDevice(), &createInfo, nullptr, &device) != VK_SUCCESS) {
        throw std::runtime_error("Failed to create logical device");
    }

    vkGetDeviceQueue(device, indices.graphicsFamily.value(), 0, &graphicsQueue);
}

ElyLogicDevice::~ElyLogicDevice() { vkDestroyDevice(device, nullptr); }

}   // namespace Ely