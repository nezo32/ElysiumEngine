#pragma once

#include <memory>
#include <vector>

#include "command/command_buffer.hpp"
#include "utils/sync.hpp"

namespace Ely {

struct ElysiumDependencies;

class Renderer {
   private:
    size_t currentFrame = 0;

    ElysiumDependencies& deps;

    std::vector<std::unique_ptr<CommandBuffer>> commandBuffers;
    std::vector<std::unique_ptr<Sync::Semaphore>> imageAvailableSemaphores;
    std::vector<std::unique_ptr<Sync::Semaphore>> renderFinishedSemaphores;
    std::vector<std::unique_ptr<Sync::Fence>> inFlightFences;

    void present(VkSemaphore* signal, uint32_t* imageIndices);
    void recreate();
    void updateUniform(size_t frameIndex);

   public:
    Renderer(ElysiumDependencies& deps);

    void Draw();
    void PollEvents();
    void WaitEvents();
    void WaitIdle();
};

}   // namespace Ely