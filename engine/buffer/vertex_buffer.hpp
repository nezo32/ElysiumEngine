#pragma once

#include <array>

#include "device/device.hpp"
#include "device/phys_device.hpp"
#include "external/glfw.hpp"
#include "external/glm.hpp"

namespace Ely {

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
};

class VertexBuffer {
   private:
    Device& device;
    VkBuffer vertexBuffer;
    VkDeviceMemory vertexBufferMemory;

    VkBufferCreateInfo bufferInfo{};
    VkMemoryRequirements memRequirements;

   public:
    VertexBuffer(PhysDevice& physDevice, Device& device, size_t verticesSize);
    ~VertexBuffer();

    void Map(std::vector<Vertex>& vertices);

    VkBuffer GetBuffer() { return vertexBuffer; }
};

}   // namespace Ely