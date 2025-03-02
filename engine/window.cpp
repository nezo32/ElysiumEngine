#include "window.hpp"

#include "dependencies.hpp"

namespace Ely {

Window::Window(ElysiumCreateInfo &createInfo) {
    if (glfwInit() == GLFW_FALSE) {
        throw std::runtime_error("Failed to initizalize GLFW");
    }

    this->setHints();

    this->width = createInfo.initial_width;
    this->height = createInfo.initial_height;
    window = glfwCreateWindow(width, height, createInfo.application_name, nullptr, nullptr);
    if (window == NULL) {
        glfwTerminate();
        throw std::runtime_error("Failed to create window");
    }

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

void Window::framebufferResizeCallback(GLFWwindow *window, int width, int height) {
    auto w = reinterpret_cast<Window *>(glfwGetWindowUserPointer(window));
    w->SetFramebufferResized(true);
    w->width = width;
    w->height = height;
}

}   // namespace Ely
