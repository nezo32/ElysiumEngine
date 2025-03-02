#include <memory>

#include "dependencies.hpp"
#include "descriptor/descriptor_builder.hpp"
#include "renderer.hpp"

namespace Ely {

struct ElysiumDependencies;

class Elysium {
   private:
    ElysiumDependencies deps{};

    std::vector<Vertex> vertices = {{{-0.5f, -0.5f, 0.f}, {1.0f, 0.0f, 0.0f}},
                                    {{0.5f, -0.5f, 0.f}, {0.0f, 1.0f, 0.0f}},
                                    {{0.5f, 0.5f, 0.f}, {0.0f, 0.0f, 1.0f}},
                                    {{-0.5f, 0.5f, 0.f}, {1.f, 1.f, 1.f}}};
    std::vector<uint32_t> indices = {0, 2, 1, 2, 0, 3};

   public:
    Renderer* renderer;
    Window* window;

    Elysium(ElysiumCreateInfo createInfo) {
        deps.window = new Window(createInfo);
        window = deps.window;

        deps.vulkan = new Vulkan(createInfo, deps);
        deps.physDevice = new PhysDevice(deps);
        deps.device = new Device(deps);
        deps.swapChain = new SwapChain(deps);
        deps.renderPass = new RenderPass(deps);

        DescriptorLayoutBuilder builder{};
        builder.AddBinding(0, VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, VK_SHADER_STAGE_VERTEX_BIT);
        deps.descriptorLayouts.push_back(new DescriptorLayout(deps, builder.GetBindings()));

        deps.pipelineLayout = new PipelineLayout(deps);

        auto vertexShaderCore = new ShaderModule(deps, "vertex_core.vert.spv");
        auto fragmentShaderCore = new ShaderModule(deps, "fragment_core.frag.spv");

        deps.pipelines.insert(std::pair<PipelineType, Pipeline*>(
            PipelineCore,
            new Pipeline(deps, vertexShaderCore, fragmentShaderCore, Pipeline::defaultPipelineConfigInfo())));

        delete vertexShaderCore;
        delete fragmentShaderCore;

        deps.frameBuffers = new FrameBuffers(deps);
        deps.commandPools.insert(std::pair<CommandPoolType, CommandPool*>(CommandPoolCore, new CommandPool(deps)));

        /* deps.uniformBuffers = new UniformBuffers(deps);
        deps.descriptorPool = new DescriptorPool(deps);
        deps.descriptorSets = new DescriptorSets(deps); */

        // TODO: move to method
        deps.meshes.push_back(new Mesh(deps, vertices, indices));

        renderer = new Renderer(deps);
    }

    ~Elysium() {
        delete deps.frameBuffers;
        delete deps.swapChain;
        for (auto& pair : deps.pipelines) {
            delete pair.second;
        }
        delete deps.pipelineLayout;
        delete deps.renderPass;
        delete deps.uniformBuffers;
        delete deps.descriptorPool;
        for (auto& layout : deps.descriptorLayouts) {
            delete layout;
        }
        for (auto& mesh : deps.meshes) {
            delete mesh;
        }
        for (auto& pair : deps.commandPools) {
            delete pair.second;
        }
        delete renderer;
        delete deps.device;
        delete deps.physDevice;
        delete deps.vulkan;
        delete deps.window;
    }
};

}   // namespace Ely