#include <cassert>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <string>

#include "pipeline.hpp"
#include "utils/debug_log.hpp"

#ifndef SHADER_DIR
#define SHADER_DIR "../../../../ElysiumEngine/shaders/"
#endif

namespace Ely {

Pipeline::Pipeline(Device &d, const PipelineConfigInfo &configInfo, const char *vertexPath, const char *fragmentPath)
    : device{d} {
    createPipeline(configInfo, vertexPath, fragmentPath);
}

Pipeline::~Pipeline() {
    vkDestroyShaderModule(device.GetDevice(), vertexModule, nullptr);
    vkDestroyShaderModule(device.GetDevice(), fragmentModule, nullptr);
    vkDestroyPipeline(device.GetDevice(), pipeline, nullptr);
}

std::vector<char> Pipeline::readFile(const char *path) {
    namespace fs = std::filesystem;
    fs::path filePath(std::string(SHADER_DIR) + path);
    std::string shaderPath = fs::canonical(filePath).generic_string();

    std::ifstream file{shaderPath, std::ios::ate | std::ios::binary};

    if (!file.is_open()) {
        throw std::runtime_error("Failed to open file: " + shaderPath);
    }

    size_t fileSize = static_cast<size_t>(file.tellg());
    std::vector<char> buffer(fileSize);

    file.seekg(0);
    file.read(buffer.data(), fileSize);

    file.close();
    return buffer;
}

void Pipeline::createShaderModule(const std::vector<char> &shader, VkShaderModule *module) {
    VkShaderModuleCreateInfo createInfo{};
    createInfo.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
    createInfo.pCode = reinterpret_cast<const uint32_t *>(shader.data());
    createInfo.codeSize = shader.size();

    if (vkCreateShaderModule(device.GetDevice(), &createInfo, nullptr, module)) {
        throw std::runtime_error("Failed to create shader module");
    }
}

void Pipeline::createPipeline(const PipelineConfigInfo &configInfo, const char *vertexPath, const char *fragmentPath) {
    assert(configInfo.pipelineLayout != VK_NULL_HANDLE && "Failed to create pipeline: no pipelineLayout provided");
    assert(configInfo.renderPass != VK_NULL_HANDLE && "Failed to create pipeline: no renderPass provided");

    auto vertex = readFile(vertexPath);
    auto fragment = readFile(fragmentPath);

#ifdef _DEBUG
    std::cout << "Vertex shader size: " << vertex.size() << std::endl;
    std::cout << "Fragment shader size: " << fragment.size() << std::endl;
#endif

    createShaderModule(vertex, &vertexModule);
    createShaderModule(fragment, &fragmentModule);

    // STAGES
    VkPipelineShaderStageCreateInfo shaderStages[2];
    shaderStages[0].sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
    shaderStages[0].stage = VK_SHADER_STAGE_VERTEX_BIT;
    shaderStages[0].module = vertexModule;
    shaderStages[0].pName = "main";
    shaderStages[0].flags = 0;
    shaderStages[0].pNext = nullptr;
    shaderStages[0].pSpecializationInfo = nullptr;

    shaderStages[1].sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
    shaderStages[1].stage = VK_SHADER_STAGE_FRAGMENT_BIT;
    shaderStages[1].module = fragmentModule;
    shaderStages[1].pName = "main";
    shaderStages[1].flags = 0;
    shaderStages[1].pNext = nullptr;
    shaderStages[1].pSpecializationInfo = nullptr;

    // VERTEX INPUT
    VkPipelineVertexInputStateCreateInfo vertexInputInfo{};
    vertexInputInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO;
    vertexInputInfo.vertexAttributeDescriptionCount = 0;
    vertexInputInfo.vertexBindingDescriptionCount = 0;
    vertexInputInfo.pVertexAttributeDescriptions = nullptr;
    vertexInputInfo.pVertexBindingDescriptions = nullptr;

    // DYNAMIC STATE
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

    pipelineInfo.layout = configInfo.pipelineLayout;
    pipelineInfo.renderPass = configInfo.renderPass;
    pipelineInfo.subpass = configInfo.subpass;

    pipelineInfo.basePipelineIndex = -1;
    pipelineInfo.basePipelineHandle = VK_NULL_HANDLE;

    if (vkCreateGraphicsPipelines(device.GetDevice(), VK_NULL_HANDLE, 1, &pipelineInfo, nullptr, &pipeline) !=
        VK_SUCCESS) {
        throw std::runtime_error("Failed to create pipeline");
    }
}

PipelineConfigInfo Pipeline::defaultPipelineConfigInfo(VkRenderPass renderPass, VkPipelineLayout pipelineLayout) {
    PipelineConfigInfo configInfo{};

    configInfo.assemblyInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO;
    configInfo.assemblyInfo.topology = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST;
    configInfo.assemblyInfo.primitiveRestartEnable = VK_FALSE;

    configInfo.rasterizationInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO;
    configInfo.rasterizationInfo.depthClampEnable = VK_FALSE;
    configInfo.rasterizationInfo.rasterizerDiscardEnable = VK_FALSE;
    configInfo.rasterizationInfo.polygonMode = VK_POLYGON_MODE_FILL;
    configInfo.rasterizationInfo.lineWidth = 1.0f;
    configInfo.rasterizationInfo.cullMode = VK_CULL_MODE_NONE;
    configInfo.rasterizationInfo.frontFace = VK_FRONT_FACE_CLOCKWISE;
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

    configInfo.renderPass = renderPass;
    configInfo.pipelineLayout = pipelineLayout;

    return configInfo;
}

void Pipeline::Bind(VkCommandBuffer commandBuffer) {
    vkCmdBindPipeline(commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, pipeline);
}

}   // namespace Ely