#pragma once

#include "device/device.hpp"
#include "external/glfw.hpp"

namespace Ely {
class PipelineLayout {
   private:
    Device& device;
    VkPipelineLayout pipelineLayout;

   public:
    PipelineLayout(Device& d);
    ~PipelineLayout();

    VkPipelineLayout GetPipelineLayout() { return pipelineLayout; }
};
}   // namespace Ely