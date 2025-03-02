#include "command_pool.hpp"

#include "dependencies.hpp"
#include "utils/queue_families.hpp"

namespace Ely {

CommandPool::CommandPool(ElysiumDependencies& deps) : deps{deps} {
    auto queueFamilyIndices = findQueueFamilies(deps.vulkan->GetSurface(), deps.physDevice->GetPhysicalDevice());

    VkCommandPoolCreateInfo poolInfo{};
    poolInfo.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
    poolInfo.flags = VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT;
    poolInfo.queueFamilyIndex = queueFamilyIndices.graphicsFamily.value();

    if (vkCreateCommandPool(deps.device->GetDevice(), &poolInfo, nullptr, &pool) != VK_SUCCESS) {
        throw std::runtime_error("failed to create command pool!");
    }
}

CommandPool::~CommandPool() { vkDestroyCommandPool(deps.device->GetDevice(), pool, nullptr); }

}   // namespace Ely