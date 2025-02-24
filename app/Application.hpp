#pragma once

#include <memory>
#include <vector>

#include "elysium/elysium.hpp"

class Application {
   public:
    void Run();

   private:
    static constexpr int WIDTH = 1280;
    static constexpr int HEIGHT = 720;
    static constexpr const char *APP_NAME = "ZALUPA";

    Ely::Elysium e{WIDTH, HEIGHT, APP_NAME, APP_NAME, Ely::Elysium::GetVersionCode(1, 0, 0)};
};