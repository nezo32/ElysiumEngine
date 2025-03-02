#pragma once

#include <array>
#include <memory>

#include "buffer.hpp"

namespace Ely {

class Buffer;
struct ElysiumDependencies;
struct SpaceTransformObject;

class UniformBuffers {
   private:
    std::vector<Buffer*> uniformBuffers;

   public:
    UniformBuffers(ElysiumDependencies& deps);
    ~UniformBuffers();

    void Update(size_t index, SpaceTransformObject* data);

    Buffer* GetUniformBuffer(uint32_t index) { return uniformBuffers[index]; }
};

}   // namespace Ely