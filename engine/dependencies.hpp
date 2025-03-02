#pragma once

#include <unordered_map>
#include <vector>

#include "buffer/uniform_buffers.hpp"
#include "command/command_pool.hpp"
#include "descriptor/descriptor_layout.hpp"
#include "descriptor/descriptor_pool.hpp"
#include "descriptor/descriptor_sets.hpp"
#include "device/device.hpp"
#include "device/phys_device.hpp"
#include "external/glfw.hpp"
#include "external/glm.hpp"
#include "frame_buffers.hpp"
#include "mesh.hpp"
#include "pipeline/pipeline.hpp"
#include "pipeline/pipeline_layout.hpp"
#include "render_pass.hpp"
#include "utils/hash.hpp"
#include "vulkan.hpp"
#include "window.hpp"

namespace Ely {

struct ElysiumApplicationVersion {
    uint32_t major;
    uint32_t minor;
    uint32_t patch;
};

struct ElysiumCreateInfo {
    int initial_width = 1280;
    int initial_height = 720;
    const char* application_name = "my_app";
    ElysiumApplicationVersion application_version{1, 0, 0};
};

struct SpaceTransformObject {
    glm::mat4 model;
    glm::mat4 view;
    glm::mat4 projection;
};

struct Vertex {
    glm::vec3 position;
    glm::vec3 color;

    static VkVertexInputBindingDescription getBindingDescription() {
        VkVertexInputBindingDescription bindingDescription{};
        bindingDescription.binding = 0;
        bindingDescription.stride = sizeof(Vertex);
        bindingDescription.inputRate = VK_VERTEX_INPUT_RATE_VERTEX;

        return bindingDescription;
    }

    static std::array<VkVertexInputAttributeDescription, 2> getAttributeDescriptions() {
        std::array<VkVertexInputAttributeDescription, 2> attributeDescriptions{};

        attributeDescriptions[0].binding = 0;
        attributeDescriptions[0].location = 0;
        attributeDescriptions[0].format = VK_FORMAT_R32G32B32_SFLOAT;
        attributeDescriptions[0].offset = offsetof(Vertex, position);

        attributeDescriptions[1].binding = 0;
        attributeDescriptions[1].location = 1;
        attributeDescriptions[1].format = VK_FORMAT_R32G32B32_SFLOAT;
        attributeDescriptions[1].offset = offsetof(Vertex, color);

        return attributeDescriptions;
    }

    bool operator==(const Vertex& other) const { return position == other.position && color == other.color; }
};

enum PipelineType {
    PipelineCore,
};

enum CommandPoolType {
    CommandPoolCore,
};

struct ElysiumDependenciesConstants {
    const uint8_t MAX_FRAMES_IN_FLIGHT = 2;
};

struct ElysiumDependenciesPaths {
    const char* shaderPath = "../../../../ElysiumEngine/shaders/";
    const char* meshPath = "../../../../ElysiumEngine/mesh/";
};

struct ElysiumDependencies {
    ElysiumDependenciesConstants consts{};
    ElysiumDependenciesPaths paths{};

    Window* window;
    Vulkan* vulkan;
    PhysDevice* physDevice;
    Device* device;
    SwapChain* swapChain;
    RenderPass* renderPass;
    std::vector<DescriptorLayout*> descriptorLayouts;
    PipelineLayout* pipelineLayout;
    std::unordered_map<PipelineType, Pipeline*> pipelines;
    FrameBuffers* frameBuffers;
    std::unordered_map<CommandPoolType, CommandPool*> commandPools;
    std::vector<Mesh*> meshes;
    UniformBuffers* uniformBuffers;
    DescriptorPool* descriptorPool;
    DescriptorSets* descriptorSets;
};

}   // namespace Ely

namespace std {
template <>
struct hash<Ely::Vertex> {
    size_t operator()(Ely::Vertex const& vertex) const {
        size_t seed = 0;
        Ely::hashCombine(seed, vertex.position, vertex.color
                         //     , vertex.normal, vertex.uv
        );
        return seed;
    }
};
}   // namespace std
