#include <cassert>
#include <iostream>
#include <string>

#include "dependencies.hpp"
#include "pipeline.hpp"

namespace Ely {

Pipeline::Pipeline(ElysiumDependencies &deps, ShaderModule *vertexShader, ShaderModule *fragmentShader,
                   const PipelineConfigInfo &configInfo)
    : deps{deps} {
    // STAGES
    VkPipelineShaderStageCreateInfo shaderStages[2];
    shaderStages[0].sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
    shaderStages[0].stage = VK_SHADER_STAGE_VERTEX_BIT;
    shaderStages[0].module = vertexShader->GetModule();
    shaderStages[0].pName = "main";
    shaderStages[0].flags = 0;
    shaderStages[0].pNext = nullptr;
    shaderStages[0].pSpecializationInfo = nullptr;

    shaderStages[1].sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
    shaderStages[1].stage = VK_SHADER_STAGE_FRAGMENT_BIT;
    shaderStages[1].module = fragmentShader->GetModule();
    shaderStages[1].pName = "main";
    shaderStages[1].flags = 0;
    shaderStages[1].pNext = nullptr;
    shaderStages[1].pSpecializationInfo = nullptr;

    // VERTEX INPUT
    VkPipelineVertexInputStateCreateInfo vertexInputInfo{};
    auto bindingDescription = Vertex::getBindingDescription();
    auto attributeDescriptions = Vertex::getAttributeDescriptions();
    vertexInputInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO;
    vertexInputInfo.vertexAttributeDescriptionCount = static_cast<uint32_t>(attributeDescriptions.size());
    vertexInputInfo.vertexBindingDescriptionCount = 1;
    vertexInputInfo.pVertexAttributeDescriptions = attributeDescriptions.data();
    vertexInputInfo.pVertexBindingDescriptions = &bindingDescription;

    // DYNAMIC STATE
    std::vector<VkDynamicState> dynamicStates = {VK_DYNAMIC_STATE_VIEWPORT, VK_DYNAMIC_STATE_SCISSOR};
    VkPipelineDynamicStateCreateInfo dynamicState{};
    dynamicState.sType = VK_STRUCTURE_TYPE_PIPELINE_DYNAMIC_STATE_CREATE_INFO;
    dynamicState.dynamicStateCount = static_cast<uint32_t>(dynamicStates.size());
    dynamicState.pDynamicStates = dynamicStates.data();

    // VIEWPORT INFO
    VkPipelineViewportStateCreateInfo viewportInfo{};
    viewportInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO;
    viewportInfo.viewportCount = 1;
    viewportInfo.scissorCount = 1;

    // PIPELINE INFO
    VkGraphicsPipelineCreateInfo pipelineInfo{};
    pipelineInfo.sType = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO;
    pipelineInfo.pInputAssemblyState = &configInfo.assemblyInfo;
    pipelineInfo.pViewportState = &viewportInfo;
    pipelineInfo.pRasterizationState = &configInfo.rasterizationInfo;
    pipelineInfo.pMultisampleState = &configInfo.multisampleInfo;
    pipelineInfo.pColorBlendState = &configInfo.blendInfo;
    pipelineInfo.pDepthStencilState = &configInfo.depthStencilInfo;
    pipelineInfo.pDynamicState = &dynamicState;

    pipelineInfo.stageCount = 2;
    pipelineInfo.pStages = shaderStages;
    pipelineInfo.pVertexInputState = &vertexInputInfo;

    pipelineInfo.layout = deps.pipelineLayout->GetPipelineLayout();
    pipelineInfo.renderPass = deps.renderPass->GetRenderPass();
    pipelineInfo.subpass = configInfo.subpass;

    pipelineInfo.basePipelineIndex = -1;
    pipelineInfo.basePipelineHandle = VK_NULL_HANDLE;

    if (vkCreateGraphicsPipelines(deps.device->GetDevice(), VK_NULL_HANDLE, 1, &pipelineInfo, nullptr, &pipeline) !=
        VK_SUCCESS) {
        throw std::runtime_error("Failed to create pipeline");
    }
}

Pipeline::~Pipeline() { vkDestroyPipeline(deps.device->GetDevice(), pipeline, nullptr); }

PipelineConfigInfo Pipeline::defaultPipelineConfigInfo() {
    PipelineConfigInfo configInfo{};

    configInfo.assemblyInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO;
    configInfo.assemblyInfo.topology = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST;
    configInfo.assemblyInfo.primitiveRestartEnable = VK_FALSE;

    configInfo.rasterizationInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO;
    configInfo.rasterizationInfo.depthClampEnable = VK_FALSE;
    configInfo.rasterizationInfo.rasterizerDiscardEnable = VK_FALSE;
    configInfo.rasterizationInfo.polygonMode = VK_POLYGON_MODE_FILL;
    configInfo.rasterizationInfo.lineWidth = 1.0f;
    configInfo.rasterizationInfo.cullMode = VK_CULL_MODE_BACK_BIT;
    configInfo.rasterizationInfo.frontFace = VK_FRONT_FACE_COUNTER_CLOCKWISE;
    configInfo.rasterizationInfo.depthBiasEnable = VK_FALSE;
    configInfo.rasterizationInfo.depthBiasConstantFactor = 0.0f;   // Optional
    configInfo.rasterizationInfo.depthBiasClamp = 0.0f;            // Optional
    configInfo.rasterizationInfo.depthBiasSlopeFactor = 0.0f;      // Optional

    configInfo.multisampleInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO;
    configInfo.multisampleInfo.sampleShadingEnable = VK_FALSE;
    configInfo.multisampleInfo.rasterizationSamples = VK_SAMPLE_COUNT_1_BIT;
    configInfo.multisampleInfo.minSampleShading = 1.0f;            // Optional
    configInfo.multisampleInfo.pSampleMask = nullptr;              // Optional
    configInfo.multisampleInfo.alphaToCoverageEnable = VK_FALSE;   // Optional
    configInfo.multisampleInfo.alphaToOneEnable = VK_FALSE;        // Optional

    configInfo.blendAttachment.colorWriteMask =
        VK_COLOR_COMPONENT_R_BIT | VK_COLOR_COMPONENT_G_BIT | VK_COLOR_COMPONENT_B_BIT | VK_COLOR_COMPONENT_A_BIT;
    configInfo.blendAttachment.blendEnable = VK_FALSE;
    configInfo.blendAttachment.srcColorBlendFactor = VK_BLEND_FACTOR_ONE;    // Optional
    configInfo.blendAttachment.dstColorBlendFactor = VK_BLEND_FACTOR_ZERO;   // Optional
    configInfo.blendAttachment.colorBlendOp = VK_BLEND_OP_ADD;               // Optional
    configInfo.blendAttachment.srcAlphaBlendFactor = VK_BLEND_FACTOR_ONE;    // Optional
    configInfo.blendAttachment.dstAlphaBlendFactor = VK_BLEND_FACTOR_ZERO;   // Optional
    configInfo.blendAttachment.alphaBlendOp = VK_BLEND_OP_ADD;               // Optional

    configInfo.blendInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO;
    configInfo.blendInfo.logicOpEnable = VK_FALSE;
    configInfo.blendInfo.logicOp = VK_LOGIC_OP_COPY;   // Optional
    configInfo.blendInfo.attachmentCount = 1;
    configInfo.blendInfo.pAttachments = &configInfo.blendAttachment;
    configInfo.blendInfo.blendConstants[0] = 0.0f;   // Optional
    configInfo.blendInfo.blendConstants[1] = 0.0f;   // Optional
    configInfo.blendInfo.blendConstants[2] = 0.0f;   // Optional
    configInfo.blendInfo.blendConstants[3] = 0.0f;   // Optional

    configInfo.depthStencilInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_DEPTH_STENCIL_STATE_CREATE_INFO;
    configInfo.depthStencilInfo.depthTestEnable = VK_TRUE;
    configInfo.depthStencilInfo.depthWriteEnable = VK_TRUE;
    configInfo.depthStencilInfo.depthCompareOp = VK_COMPARE_OP_LESS;
    configInfo.depthStencilInfo.depthBoundsTestEnable = VK_FALSE;
    configInfo.depthStencilInfo.minDepthBounds = 0.0f;   // Optional
    configInfo.depthStencilInfo.maxDepthBounds = 1.0f;   // Optional
    configInfo.depthStencilInfo.stencilTestEnable = VK_FALSE;
    configInfo.depthStencilInfo.front = {};   // Optional
    configInfo.depthStencilInfo.back = {};    // Optional

    return configInfo;
}

void Pipeline::Bind(VkCommandBuffer commandBuffer) {
    vkCmdBindPipeline(commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, pipeline);
}

}   // namespace Ely