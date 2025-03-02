#include "descriptor_layout.hpp"

#include "dependencies.hpp"

namespace Ely {

DescriptorLayout::DescriptorLayout(ElysiumDependencies &deps, std::vector<VkDescriptorSetLayoutBinding> &bindings)
    : deps{deps} {
    VkDescriptorSetLayoutCreateInfo layoutInfo{};
    layoutInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO;
    layoutInfo.bindingCount = static_cast<uint32_t>(bindings.size());
    layoutInfo.pBindings = bindings.data();

    if (vkCreateDescriptorSetLayout(deps.device->GetDevice(), &layoutInfo, nullptr, &descriptorLayout) != VK_SUCCESS) {
        throw std::runtime_error("Failed to create descriptor layout");
    }
}

DescriptorLayout::~DescriptorLayout() {
    vkDestroyDescriptorSetLayout(deps.device->GetDevice(), descriptorLayout, nullptr);
}

}   // namespace Ely