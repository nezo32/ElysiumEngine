#pragma once

#include <memory>
#include <vector>

#include "external/glfw.hpp"

namespace Ely {

struct ElysiumDependencies;

class FrameBuffers {
   private:
    ElysiumDependencies& deps;
    std::vector<VkFramebuffer> frameBuffers;

   public:
    FrameBuffers(ElysiumDependencies& deps);
    ~FrameBuffers();

    std::vector<VkFramebuffer>& GetFrameBuffers() { return frameBuffers; }
};

}   // namespace Ely