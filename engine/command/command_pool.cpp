#include "command_pool.hpp"

namespace Ely {

CommandPool::CommandPool(Vulkan& vulkan, PhysDevice& physDevice, Device& device) : device{device} {
    auto queueFamilyIndices = QueueFamilies::FindQueueFamilies(vulkan.GetSurface(), physDevice.GetPhysicalDevice());

    VkCommandPoolCreateInfo poolInfo{};
    poolInfo.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
    poolInfo.flags = VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT;
    poolInfo.queueFamilyIndex = queueFamilyIndices.graphicsFamily.value();

    if (vkCreateCommandPool(device.GetDevice(), &poolInfo, nullptr, &commandPool) != VK_SUCCESS) {
        throw std::runtime_error("failed to create command pool!");
    }
}

CommandPool::~CommandPool() { vkDestroyCommandPool(device.GetDevice(), commandPool, nullptr); }

}   // namespace Ely