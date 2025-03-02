#pragma once

#include <vector>

#include "external/glfw.hpp"
#include "shader.hpp"


namespace Ely {
struct ElysiumDependencies;

struct PipelineConfigInfo {
    VkPipelineInputAssemblyStateCreateInfo assemblyInfo;
    VkPipelineRasterizationStateCreateInfo rasterizationInfo;
    VkPipelineMultisampleStateCreateInfo multisampleInfo;
    VkPipelineColorBlendAttachmentState blendAttachment;
    VkPipelineColorBlendStateCreateInfo blendInfo;
    VkPipelineDepthStencilStateCreateInfo depthStencilInfo;
    uint32_t subpass = 0;
};

class Pipeline {
   private:
    ElysiumDependencies &deps;
    VkPipeline pipeline;

   public:
    Pipeline(ElysiumDependencies &deps, ShaderModule *vertexShader, ShaderModule *fragmentShader,
             const PipelineConfigInfo &configInfo);

    ~Pipeline();

    Pipeline(const Pipeline &) = delete;
    Pipeline &operator=(const Pipeline &) = delete;

    static PipelineConfigInfo defaultPipelineConfigInfo();

    void Bind(VkCommandBuffer commandBuffer);
};

}   // namespace Ely