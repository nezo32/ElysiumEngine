#pragma once

#include <vector>

#include "external/glfw.hpp"


namespace Ely {

struct ElysiumDependencies;

class DescriptorLayout {
   private:
    ElysiumDependencies &deps;
    VkDescriptorSetLayout descriptorLayout;

   public:
    DescriptorLayout(ElysiumDependencies &deps, std::vector<VkDescriptorSetLayoutBinding> &bindings);
    ~DescriptorLayout();

    VkDescriptorSetLayout GetDescriptorLayout() { return descriptorLayout; }
};

}   // namespace Ely