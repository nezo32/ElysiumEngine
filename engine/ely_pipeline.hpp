#pragma once

#include <vector>

#include "ely_device.hpp"

namespace Ely {

struct PipelineConfigInfo {
    VkViewport viewport;
    VkRect2D scissor;
    VkPipelineInputAssemblyStateCreateInfo assemblyInfo;
    VkPipelineRasterizationStateCreateInfo rasterizationInfo;
    VkPipelineMultisampleStateCreateInfo multisampleInfo;
    VkPipelineColorBlendAttachmentState blendAttachment;
    VkPipelineColorBlendStateCreateInfo blendInfo;
    VkPipelineDepthStencilStateCreateInfo depthStencilInfo;
    VkPipelineLayout pipelineLayout = nullptr;
    VkRenderPass renderPass = nullptr;
    uint32_t subpass = 0;
};

class ElyPipeline {
   private:
    static std::vector<char> readFile(const char *path);
    void createPipeline(const PipelineConfigInfo &configInfo, const char *vertexPath, const char *fragmentPath);

    void createShaderModule(const std::vector<char> &shader, VkShaderModule *module);

    ElyDevice &device;
    VkPipeline pipeline;
    VkShaderModule vertexModule;
    VkShaderModule fragmentModule;

   public:
    ElyPipeline(ElyDevice &device, const PipelineConfigInfo &configInfo, const char *vertexPath,
                const char *fragmentPath);

    ~ElyPipeline();

    ElyPipeline(const ElyPipeline &) = delete;
    ElyPipeline &operator=(const ElyPipeline &) = delete;

    static PipelineConfigInfo defaultPipelineConfigInfo(uint32_t width, uint32_t height);

    void Bind(VkCommandBuffer commandBuffer);
};

}   // namespace Ely