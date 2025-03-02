#include "Application.hpp"

void Application::Run() {
    while (!e.window->ShouldClose()) {
        e.renderer->PollEvents();

        e.renderer->Draw();
    }
    e.renderer->WaitIdle();
}