#include "pipeline_layout.hpp"

namespace Ely {

PipelineLayout::PipelineLayout(Device& d) : device{d} {
    VkPipelineLayoutCreateInfo pipelineLayoutInfo{};
    pipelineLayoutInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
    pipelineLayoutInfo.setLayoutCount = 0;              // Optional
    pipelineLayoutInfo.pSetLayouts = nullptr;           // Optional
    pipelineLayoutInfo.pushConstantRangeCount = 0;      // Optional
    pipelineLayoutInfo.pPushConstantRanges = nullptr;   // Optional

    if (vkCreatePipelineLayout(device.GetDevice(), &pipelineLayoutInfo, nullptr, &pipelineLayout) != VK_SUCCESS) {
        throw std::runtime_error("failed to create pipeline layout!");
    }
}

PipelineLayout::~PipelineLayout() { vkDestroyPipelineLayout(device.GetDevice(), pipelineLayout, nullptr); }

}   // namespace Ely