#pragma once

#include <vector>

#include "external/glfw.hpp"

namespace Ely {

struct ElysiumDependencies;

class DescriptorSets {
   private:
    std::vector<VkDescriptorSet> descriptorSets;

   public:
    DescriptorSets(ElysiumDependencies& deps);

    std::vector<VkDescriptorSet>& GetDescriptorSets() { return descriptorSets; }
};

}   // namespace Ely