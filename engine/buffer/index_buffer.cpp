#include "index_buffer.hpp"

#include "dependencies.hpp"

namespace Ely {

IndexBuffer::IndexBuffer(ElysiumDependencies& deps, std::vector<uint32_t>& indices) {
    VkDeviceSize bufferSize = sizeof(uint32_t) * indices.size();
    indicesCount = static_cast<uint32_t>(indices.size());

    Buffer stagingBuffer{deps, bufferSize, VK_BUFFER_USAGE_TRANSFER_SRC_BIT,
                         VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT};

    stagingBuffer.MapCopyFrom(indices.data());

    indexBuffer =
        std::make_unique<Buffer>(deps, bufferSize, VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_INDEX_BUFFER_BIT,
                                 VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT);

    Buffer::Copy(deps, stagingBuffer, *indexBuffer);
}

}   // namespace Ely