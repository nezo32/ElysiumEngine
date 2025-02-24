#include "window.hpp"

namespace Ely {

Window::Window(const int width, const int height, const char *title) {
    if (glfwInit() == GLFW_FALSE) {
        throw std::runtime_error("Failed to initizalize GLFW");
    }

    this->setHints();

    window = glfwCreateWindow(width, height, title, nullptr, nullptr);
    if (window == NULL) {
        glfwTerminate();
        throw std::runtime_error("Failed to create window");
    }
    this->width = width;
    this->height = height;

    glfwSetFramebufferSizeCallback(window, framebufferResizeCallback);
    glfwSetWindowUserPointer(window, this);
}

Window::~Window() {
    glfwDestroyWindow(window);
    glfwTerminate();
}

void Window::setHints() {
    glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
    glfwWindowHint(GLFW_RESIZABLE, GLFW_TRUE);
}

void Window::CreateWindowSurface(VkInstance instance, VkSurfaceKHR *surface) {
    if (glfwCreateWindowSurface(instance, window, nullptr, surface) != VK_SUCCESS) {
        throw std::runtime_error("Failed to create window surface");
    }
}

void Window::framebufferResizeCallback(GLFWwindow *window, int width, int height) {
    auto w = reinterpret_cast<Window *>(glfwGetWindowUserPointer(window));
    w->setFramebufferResized(true);
    w->width = width;
    w->height = height;
}

}   // namespace Ely
