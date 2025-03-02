#pragma once

#include <array>
#include <memory>

#include "buffer.hpp"

namespace Ely {

struct Vertex;
struct ElysiumDependencies;

class VertexBuffer {
   private:
    std::unique_ptr<Buffer> vertexBuffer;
    uint32_t verticesCount;

   public:
    VertexBuffer(ElysiumDependencies& deps, std::vector<Vertex>& vertices);

    VkBuffer GetVertexBuffer() { return vertexBuffer->GetBuffer(); }
    uint32_t GetVerticesCount() { return verticesCount; }
};

}   // namespace Ely