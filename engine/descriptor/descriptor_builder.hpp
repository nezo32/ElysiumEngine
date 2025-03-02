#pragma once

#include <vector>

#include "external/glfw.hpp"

namespace Ely {

class DescriptorLayoutBuilder {
   private:
    std::vector<VkDescriptorSetLayoutBinding> bindings;

   public:
    void AddBinding(uint32_t binding, VkDescriptorType type, VkShaderStageFlags flags, VkSampler* samplers = nullptr);
    std::vector<VkDescriptorSetLayoutBinding>& GetBindings() { return bindings; }
};

}   // namespace Ely
