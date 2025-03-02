#include "uniform_buffers.hpp"

#include "dependencies.hpp"

namespace Ely {

UniformBuffers::UniformBuffers(ElysiumDependencies& deps) {
    for (uint8_t i = 0; i < deps.consts.MAX_FRAMES_IN_FLIGHT; i++) {
        uniformBuffers.push_back(
            new Buffer(deps, sizeof(SpaceTransformObject), VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT,
                       VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT));

        vkMapMemory(deps.device->GetDevice(), uniformBuffers[i]->GetBufferMemory(), 0, sizeof(SpaceTransformObject), 0,
                    uniformBuffers[i]->GetMappedBuffer());
    }
}

UniformBuffers::~UniformBuffers() {
    for (auto& buffer : uniformBuffers) {
        delete buffer;
    }
}

void UniformBuffers::Update(size_t index, SpaceTransformObject* data) {
    memcpy(uniformBuffers[index]->GetMappedBuffer(), data, sizeof(SpaceTransformObject));
}

}   // namespace Ely