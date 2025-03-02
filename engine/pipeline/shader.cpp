#include "shader.hpp"

#include <filesystem>
#include <fstream>

#include "dependencies.hpp"

namespace Ely {

ShaderModule::ShaderModule(ElysiumDependencies &deps, const char *path) : deps{deps} {
    auto shader = readFile(path, deps.paths.shaderPath);

    VkShaderModuleCreateInfo createInfo{};
    createInfo.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
    createInfo.pCode = reinterpret_cast<const uint32_t *>(shader.data());
    createInfo.codeSize = shader.size();

    if (vkCreateShaderModule(deps.device->GetDevice(), &createInfo, nullptr, &shaderModule)) {
        throw std::runtime_error("Failed to create shader module");
    }
}

ShaderModule::~ShaderModule() { vkDestroyShaderModule(deps.device->GetDevice(), shaderModule, nullptr); }

std::vector<char> ShaderModule::readFile(const char *path, const char *basePath) {
    namespace fs = std::filesystem;
    fs::path filePath(std::string(basePath) + path);
    std::string shaderPath = fs::canonical(filePath).generic_string();

    std::ifstream file{shaderPath, std::ios::ate | std::ios::binary};

    if (!file.is_open()) {
        throw std::runtime_error("Failed to open file: " + shaderPath);
    }

    size_t fileSize = static_cast<size_t>(file.tellg());
    std::vector<char> buffer(fileSize);

    file.seekg(0);
    file.read(buffer.data(), fileSize);

    file.close();
    return buffer;
}

}   // namespace Ely