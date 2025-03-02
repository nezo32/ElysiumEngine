#pragma once

#include "external/glfw.hpp"

namespace Ely {

struct ElysiumDependencies;

class PipelineLayout {
   private:
    ElysiumDependencies &deps;
    VkPipelineLayout pipelineLayout;

   public:
    PipelineLayout(ElysiumDependencies &deps);
    ~PipelineLayout();

    VkPipelineLayout GetPipelineLayout() { return pipelineLayout; }
};
}   // namespace Ely