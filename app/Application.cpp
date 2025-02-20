#include "Application.hpp"

void Application::Run() {
  while (!elyWindow.ShouldClose()) {
    Ely::ElyEvents::PollEvents();
  }
}