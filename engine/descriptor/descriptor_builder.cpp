#include "descriptor_builder.hpp"

#include "dependencies.hpp"

namespace Ely {

void DescriptorLayoutBuilder::AddBinding(uint32_t binding, VkDescriptorType type, VkShaderStageFlags flags,
                                         VkSampler* samplers) {
    VkDescriptorSetLayoutBinding layoutBinding{};
    layoutBinding.binding = binding;
    layoutBinding.descriptorType = type;
    layoutBinding.descriptorCount = 1;
    layoutBinding.stageFlags = flags;
    layoutBinding.pImmutableSamplers = samplers;

    bindings.push_back(layoutBinding);
}

}   // namespace Ely
