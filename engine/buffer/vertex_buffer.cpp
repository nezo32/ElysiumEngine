#include "vertex_buffer.hpp"

namespace Ely {

VertexBuffer::VertexBuffer(PhysDevice& physDevice, Device& device, size_t verticesSize) : device{device} {
    bufferInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
    bufferInfo.size = sizeof(Vertex) * verticesSize;
    bufferInfo.usage = VK_BUFFER_USAGE_VERTEX_BUFFER_BIT;
    bufferInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;

    if (vkCreateBuffer(device.GetDevice(), &bufferInfo, nullptr, &vertexBuffer) != VK_SUCCESS) {
        throw std::runtime_error("Failed to create vertex buffer");
    }

    vkGetBufferMemoryRequirements(device.GetDevice(), vertexBuffer, &memRequirements);

    VkMemoryAllocateInfo allocInfo{};
    allocInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
    allocInfo.allocationSize = memRequirements.size;
    allocInfo.memoryTypeIndex = physDevice.FindMemoryType(
        memRequirements.memoryTypeBits, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT);
    if (vkAllocateMemory(device.GetDevice(), &allocInfo, nullptr, &vertexBufferMemory) != VK_SUCCESS) {
        throw std::runtime_error("Failed to allocate vertex buffer memory");
    }

    vkBindBufferMemory(device.GetDevice(), vertexBuffer, vertexBufferMemory, 0);
}

VertexBuffer::~VertexBuffer() {
    vkDestroyBuffer(device.GetDevice(), vertexBuffer, nullptr);
    vkFreeMemory(device.GetDevice(), vertexBufferMemory, nullptr);
}

void VertexBuffer::Map(std::vector<Vertex>& vertices) {
    void* data;
    vkMapMemory(device.GetDevice(), vertexBufferMemory, 0, bufferInfo.size, 0, &data);
    memcpy(data, vertices.data(), (size_t) bufferInfo.size);
    vkUnmapMemory(device.GetDevice(), vertexBufferMemory);
}

}   // namespace Ely