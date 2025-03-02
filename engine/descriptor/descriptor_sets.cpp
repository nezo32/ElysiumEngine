#include "descriptor_sets.hpp"

#include "dependencies.hpp"

namespace Ely {

DescriptorSets::DescriptorSets(ElysiumDependencies& deps) {
    std::vector<VkDescriptorSetLayout> layouts(deps.consts.MAX_FRAMES_IN_FLIGHT,
                                               deps.descriptorLayouts[0]->GetDescriptorLayout());
    VkDescriptorSetAllocateInfo allocInfo{};
    allocInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
    allocInfo.descriptorPool = deps.descriptorPool->GetDescriptorPool();
    allocInfo.descriptorSetCount = static_cast<uint32_t>(deps.consts.MAX_FRAMES_IN_FLIGHT);
    allocInfo.pSetLayouts = layouts.data();

    descriptorSets.resize(deps.consts.MAX_FRAMES_IN_FLIGHT);
    if (vkAllocateDescriptorSets(deps.device->GetDevice(), &allocInfo, descriptorSets.data()) != VK_SUCCESS) {
        throw std::runtime_error("failed to allocate descriptor sets!");
    }

    for (uint8_t i = 0; i < deps.consts.MAX_FRAMES_IN_FLIGHT; i++) {
        VkDescriptorBufferInfo bufferInfo{};
        bufferInfo.buffer = deps.uniformBuffers->GetUniformBuffer(i)->GetBuffer();
        bufferInfo.offset = 0;
        bufferInfo.range = sizeof(SpaceTransformObject);

        VkWriteDescriptorSet descriptorWrite{};
        descriptorWrite.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
        descriptorWrite.dstSet = descriptorSets[i];
        descriptorWrite.dstBinding = 0;
        descriptorWrite.dstArrayElement = 0;
        descriptorWrite.descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
        descriptorWrite.descriptorCount = 1;
        descriptorWrite.pBufferInfo = &bufferInfo;

        vkUpdateDescriptorSets(deps.device->GetDevice(), 1, &descriptorWrite, 0, nullptr);
    }
}

}   // namespace Ely