#pragma once

#include <vector>

#include "device/device.hpp"
#include "external/glfw.hpp"

namespace Ely {

struct PipelineConfigInfo {
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

    static inline std::vector<VkDynamicState> dynamicStates = {VK_DYNAMIC_STATE_VIEWPORT, VK_DYNAMIC_STATE_SCISSOR};

   public:
    Pipeline(Device &device, const PipelineConfigInfo &configInfo, const char *vertexPath, const char *fragmentPath);

    ~Pipeline();

    Pipeline(const Pipeline &) = delete;
    Pipeline &operator=(const Pipeline &) = delete;

    static PipelineConfigInfo defaultPipelineConfigInfo(VkRenderPass renderPass, VkPipelineLayout pipelineLayout);

    void Bind(VkCommandBuffer commandBuffer);
};

}   // namespace Ely