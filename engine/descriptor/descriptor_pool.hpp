#pragma once

#include "external/glfw.hpp"

namespace Ely {

struct ElysiumDependencies;

class DescriptorPool {
   private:
    ElysiumDependencies& deps;
    VkDescriptorPool descriptorPool;

   public:
    DescriptorPool(ElysiumDependencies& deps);
    ~DescriptorPool();

    VkDescriptorPool GetDescriptorPool() { return descriptorPool; }
};

}   // namespace Ely