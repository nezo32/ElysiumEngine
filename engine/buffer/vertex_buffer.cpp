#include "vertex_buffer.hpp"

#include "dependencies.hpp"

namespace Ely {

VertexBuffer::VertexBuffer(ElysiumDependencies& deps, std::vector<Vertex>& vertices) {
    VkDeviceSize bufferSize = sizeof(Vertex) * vertices.size();
    verticesCount = static_cast<uint32_t>(vertices.size());

    Buffer stagingBuffer{deps, bufferSize, VK_BUFFER_USAGE_TRANSFER_SRC_BIT,
                         VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT};

    stagingBuffer.MapCopyFrom(vertices.data());

    vertexBuffer =
        std::make_unique<Buffer>(deps, bufferSize, VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_VERTEX_BUFFER_BIT,
                                 VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT);

    Buffer::Copy(deps, stagingBuffer, *vertexBuffer);
}

}   // namespace Ely