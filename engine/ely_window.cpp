#include "ely_window.h"

namespace Ely {

ElyWindow::ElyWindow(const int &width, const int &height, const char *title) {
    if (glfwInit() == GLFW_FALSE) {
        throw std::runtime_error("ERROR :: ElyWindow :: Failed to initizalize GLFW");
    }

    this->setHints();

    window = glfwCreateWindow(width, height, title, nullptr, nullptr);
    if (window == NULL) {
        glfwTerminate();
        throw std::runtime_error("ERROR :: ElyWindow :: Failed to create window");
    }
    this->width = width;
    this->height = height;
}

ElyWindow::~ElyWindow() {
    glfwDestroyWindow(window);
    glfwTerminate();
}

void ElyWindow::setHints() {
    glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
    glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);
}

void ElyWindow::CreateWindowSurface(VkInstance instance, VkSurfaceKHR *surface) {
    if (glfwCreateWindowSurface(instance, window, nullptr, surface) != VK_SUCCESS)
        throw std::runtime_error("ERROR :: ElyWindow :: Failed to create window surface");
}

}   // namespace Ely
