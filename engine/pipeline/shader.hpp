#pragma once

#include <vector>

#include "external/glfw.hpp"


namespace Ely {

struct ElysiumDependencies;

class ShaderModule {
   private:
    ElysiumDependencies &deps;
    VkShaderModule shaderModule;
    std::vector<char> readFile(const char *path, const char *basePath = "");

   public:
    ShaderModule(ElysiumDependencies &deps, const char *path);
    ~ShaderModule();

    VkShaderModule GetModule() { return shaderModule; }
};

}   // namespace Ely