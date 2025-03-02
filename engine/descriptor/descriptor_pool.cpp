#include "descriptor_pool.hpp"

#include "dependencies.hpp"

namespace Ely {

DescriptorPool::DescriptorPool(ElysiumDependencies& deps) : deps{deps} {
    VkDescriptorPoolSize poolSize{};
    poolSize.type = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
    poolSize.descriptorCount = deps.consts.MAX_FRAMES_IN_FLIGHT;

    VkDescriptorPoolCreateInfo poolInfo{};
    poolInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
    poolInfo.poolSizeCount = 1;
    poolInfo.pPoolSizes = &poolSize;
    poolInfo.maxSets = deps.consts.MAX_FRAMES_IN_FLIGHT;

    if (vkCreateDescriptorPool(deps.device->GetDevice(), &poolInfo, nullptr, &descriptorPool) != VK_SUCCESS) {
        throw std::runtime_error("failed to create descriptor pool!");
    }
}

DescriptorPool::~DescriptorPool() { vkDestroyDescriptorPool(deps.device->GetDevice(), descriptorPool, nullptr); }

}   // namespace Ely