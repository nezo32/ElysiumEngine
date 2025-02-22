

#include "ely_device.hpp"
#include "ely_events.hpp"
#include "ely_pipeline.hpp"
#include "ely_window.hpp"

namespace Ely {
class Elysium {
   public:
    Elysium(const int& windowWidth, const int& windowHeight, const char* windowTitle, const char* appName,
            const uint32_t appVersion)
        : window{windowWidth, windowHeight, windowTitle}, vulkan{window, appName, appVersion} {}

    static uint32_t GetVersionCode(uint32_t major, uint32_t minor, uint32_t patch) {
        return VK_MAKE_VERSION(major, minor, patch);
    }

    // WINDOW
    Ely::Window window;

    // VULKAN INSTANCE
    Ely::Vulkan vulkan;

    // PHYSICAL DEVICE
    Ely::PhysDevice physDevice{vulkan};

    // LOGICAL DEVICE
    Ely::Device device{vulkan, physDevice};

    // SWAP CHAIN
    Ely::SwapChain swapChain{window, vulkan, physDevice, device};
};
}   // namespace Ely