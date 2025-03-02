#pragma once

#include <cassert>
#include <vector>

#include "external/glfw.hpp"

namespace Ely {

struct ElysiumDependencies;

class Buffer {
   private:
    ElysiumDependencies& deps;
    VkBuffer buffer;
    VkDeviceMemory bufferMemory;
    void* bufferMapped;
    VkDeviceSize size;

   public:
    Buffer(ElysiumDependencies& deps, VkDeviceSize size, VkBufferUsageFlags usage, VkMemoryPropertyFlags properties);
    ~Buffer();

    void MapCopyFrom(void* data);
    void Map();

    VkBuffer GetBuffer() { return buffer; }
    VkDeviceMemory GetBufferMemory() { return bufferMemory; }
    void** GetMappedBuffer() { return &bufferMapped; }

    static void Copy(ElysiumDependencies& deps, Buffer& src, Buffer& dst);
};

}   // namespace Ely