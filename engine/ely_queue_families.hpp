#include <optional>

#include "external/ely_glfw.hpp"

namespace Ely {
struct QueueFamilyIndices {
    std::optional<uint32_t> graphicsFamily;
    std::optional<uint32_t> presentFamily;

    bool isComplete() { return graphicsFamily.has_value() && presentFamily.has_value(); }
};

class QueueFamilies {
   public:
    static QueueFamilyIndices FindQueueFamilies(VkSurfaceKHR surface, VkPhysicalDevice device);
};
}   // namespace Ely