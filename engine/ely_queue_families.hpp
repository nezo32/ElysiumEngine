#include <optional>

#include "external/ely_glfw.hpp"

namespace Ely {
struct QueueFamilyIndices {
    std::optional<uint32_t> graphicsFamily;

    bool isComplete() { return graphicsFamily.has_value(); }
};

class ElyQueueFamilies {
   public:
    static QueueFamilyIndices FindQueueFamilies(VkPhysicalDevice device);
};
}   // namespace Ely