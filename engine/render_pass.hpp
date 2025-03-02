#pragma once

#include <memory>

#include "device/device.hpp"
#include "external/glfw.hpp"
#include "swap_chain.hpp"

namespace Ely {

struct ElysiumDependencies;
class RenderPass {
   private:
    ElysiumDependencies &deps;
    VkRenderPass renderPass;

   public:
    RenderPass(ElysiumDependencies &deps);
    ~RenderPass();

    VkRenderPass GetRenderPass() { return renderPass; }
};
}   // namespace Ely