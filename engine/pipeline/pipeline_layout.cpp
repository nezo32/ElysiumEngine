#include "pipeline_layout.hpp"

#include "dependencies.hpp"

namespace Ely {

PipelineLayout::PipelineLayout(ElysiumDependencies& deps) : deps{deps} {
    std::vector<VkDescriptorSetLayout> layouts;
    for (auto& layout : deps.descriptorLayouts) {
        layouts.push_back(layout->GetDescriptorLayout());
    }

    VkPipelineLayoutCreateInfo pipelineLayoutInfo{};
    pipelineLayoutInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
    pipelineLayoutInfo.setLayoutCount = static_cast<uint32_t>(layouts.size());
    pipelineLayoutInfo.pSetLayouts = layouts.data();
    pipelineLayoutInfo.pushConstantRangeCount = 0;
    pipelineLayoutInfo.pPushConstantRanges = nullptr;

    if (vkCreatePipelineLayout(deps.device->GetDevice(), &pipelineLayoutInfo, nullptr, &pipelineLayout) != VK_SUCCESS) {
        throw std::runtime_error("failed to create pipeline layout!");
    }
}

PipelineLayout::~PipelineLayout() { vkDestroyPipelineLayout(deps.device->GetDevice(), pipelineLayout, nullptr); }

}   // namespace Ely