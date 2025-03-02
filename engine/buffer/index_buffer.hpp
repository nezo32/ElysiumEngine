#pragma once

#include <memory>

#include "buffer.hpp"

namespace Ely {

class Buffer;
struct ElysiumDependencies;

class IndexBuffer {
   private:
    std::unique_ptr<Buffer> indexBuffer;
    uint32_t indicesCount;

   public:
    IndexBuffer(ElysiumDependencies& deps, std::vector<uint32_t>& indices);

    VkBuffer GetIndexBuffer() { return indexBuffer->GetBuffer(); }
    uint32_t GetIndicesCount() { return indicesCount; }
};

}   // namespace Ely