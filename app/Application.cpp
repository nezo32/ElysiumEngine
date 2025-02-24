#include "Application.hpp"

#include <array>

void Application::Run() {
    while (!e.window.ShouldClose()) {
        Ely::Events::PollEvents();
        e.renderer.Draw();
    }
    Ely::Events::WaitIdle(e.device);
}