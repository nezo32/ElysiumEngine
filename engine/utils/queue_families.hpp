#include <optional>

#include "external/glfw.hpp"

namespace Ely {
struct QueueFamilyIndices {
    std::optional<uint32_t> graphicsFamily;
    std::optional<uint32_t> presentFamily;

    bool isComplete() { return graphicsFamily.has_value() && presentFamily.has_value(); }
};

QueueFamilyIndices findQueueFamilies(VkSurfaceKHR surface, VkPhysicalDevice device);

}   // namespace Ely