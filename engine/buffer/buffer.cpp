#include "buffer.hpp"

#include "dependencies.hpp"

namespace Ely {

Buffer::Buffer(ElysiumDependencies& deps, VkDeviceSize size, VkBufferUsageFlags usage, VkMemoryPropertyFlags properties)
    : deps{deps} {
    this->size = size;

    VkBufferCreateInfo bufferInfo{};
    bufferInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
    bufferInfo.size = size;
    bufferInfo.usage = usage;
    bufferInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;

    if (vkCreateBuffer(deps.device->GetDevice(), &bufferInfo, nullptr, &buffer) != VK_SUCCESS) {
        throw std::runtime_error("failed to create buffer!");
    }

    VkMemoryRequirements memRequirements;
    vkGetBufferMemoryRequirements(deps.device->GetDevice(), buffer, &memRequirements);

    VkMemoryAllocateInfo allocInfo{};
    allocInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
    allocInfo.allocationSize = memRequirements.size;
    allocInfo.memoryTypeIndex = deps.physDevice->FindMemoryType(memRequirements.memoryTypeBits, properties);

    //
    /*
     * TODO
     * The right way to allocate memory for a large number of objects at the same time is to create a custom allocator
     * that splits up a single allocation among many different objects by using the offset parameters that we’ve seen in
     * many functions.
     */
    if (vkAllocateMemory(deps.device->GetDevice(), &allocInfo, nullptr, &bufferMemory) != VK_SUCCESS) {
        throw std::runtime_error("failed to allocate buffer memory!");
    }

    vkBindBufferMemory(deps.device->GetDevice(), buffer, bufferMemory, 0);
}

Buffer::~Buffer() {
    vkDestroyBuffer(deps.device->GetDevice(), buffer, nullptr);
    vkFreeMemory(deps.device->GetDevice(), bufferMemory, nullptr);
}

void Buffer::Copy(ElysiumDependencies& deps, Buffer& src, Buffer& dst) {
    VkCommandBufferAllocateInfo allocInfo{};
    allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
    allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
    allocInfo.commandPool = deps.commandPools.at(CommandPoolCore)->GetCommandPool();
    allocInfo.commandBufferCount = 1;

    VkCommandBuffer commandBuffer;
    vkAllocateCommandBuffers(deps.device->GetDevice(), &allocInfo, &commandBuffer);

    VkCommandBufferBeginInfo beginInfo{};
    beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
    beginInfo.flags = VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT;

    vkBeginCommandBuffer(commandBuffer, &beginInfo);

    VkBufferCopy copyRegion{};
    copyRegion.srcOffset = 0;   // Optional
    copyRegion.dstOffset = 0;   // Optional
    copyRegion.size = dst.size;
    vkCmdCopyBuffer(commandBuffer, src.buffer, dst.buffer, 1, &copyRegion);

    vkEndCommandBuffer(commandBuffer);

    VkSubmitInfo submitInfo{};
    submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
    submitInfo.commandBufferCount = 1;
    submitInfo.pCommandBuffers = &commandBuffer;

    vkQueueSubmit(deps.device->GetGraphicsQueue(), 1, &submitInfo, VK_NULL_HANDLE);
    vkQueueWaitIdle(deps.device->GetGraphicsQueue());

    vkFreeCommandBuffers(deps.device->GetDevice(), deps.commandPools.at(CommandPoolCore)->GetCommandPool(), 1,
                         &commandBuffer);
}

void Buffer::MapCopyFrom(void* data) {
    void* tmp;
    vkMapMemory(deps.device->GetDevice(), bufferMemory, 0, size, 0, &tmp);
    memcpy(tmp, data, (size_t) size);
    vkUnmapMemory(deps.device->GetDevice(), bufferMemory);
}

void Buffer::Map() { vkMapMemory(deps.device->GetDevice(), bufferMemory, 0, size, 0, &bufferMapped); }

}   // namespace Ely