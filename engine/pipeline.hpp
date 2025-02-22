#pragma once

#include <vector>

#include "device.hpp"
#include "external/glfw.hpp"

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

class Pipeline {
   private:
    static std::vector<char> readFile(const char *path);
    void createPipeline(const PipelineConfigInfo &configInfo, const char *vertexPath, const char *fragmentPath);

    void createShaderModule(const std::vector<char> &shader, VkShaderModule *module);

    Device &device;
    VkPipeline pipeline;
    VkShaderModule vertexModule;
    VkShaderModule fragmentModule;

   public:
    Pipeline(Device &device, const PipelineConfigInfo &configInfo, const char *vertexPath, const char *fragmentPath);

    ~Pipeline();

    Pipeline(const Pipeline &) = delete;
    Pipeline &operator=(const Pipeline &) = delete;

    static PipelineConfigInfo defaultPipelineConfigInfo(uint32_t width, uint32_t height);

    void Bind(VkCommandBuffer commandBuffer);
};

}   // namespace Ely