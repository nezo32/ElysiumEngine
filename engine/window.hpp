#pragma once

#include <iostream>

#include "external/glfw.hpp"

namespace Ely {
class Window {
   private:
    GLFWwindow *window;

    int width;
    int height;
    bool framebufferResized = false;

    void setHints();
    static void framebufferResizeCallback(GLFWwindow *window, int width, int height);

   public:
    Window(const int width, const int height, const char *title);
    ~Window();

    Window(const Window &) = delete;
    Window &operator=(const Window &) = delete;

    GLFWwindow *GetWindow() { return window; }

    VkExtent2D GetExtent() { return {static_cast<uint32_t>(width), static_cast<uint32_t>(height)}; }

    void setFramebufferResized(bool value) { framebufferResized = value; }
    bool IsFramebufferResized() { return framebufferResized; }

    void CreateWindowSurface(VkInstance instance, VkSurfaceKHR *surface);
    bool ShouldClose() { return glfwWindowShouldClose(window); }
    void Close() { return glfwSetWindowShouldClose(window, true); }
};
}   // namespace Ely
