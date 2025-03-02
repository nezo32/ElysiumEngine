#pragma once

#include <memory>
#include <vector>

#include "elysium/elysium.hpp"

class Application {
   public:
    void Run();

   private:
    static constexpr int WIDTH = 800;
    static constexpr int HEIGHT = 600;
    static constexpr const char *APP_NAME = "ZALUPA";

    Ely::ElysiumCreateInfo engineInfo{WIDTH, HEIGHT, APP_NAME, Ely::ElysiumApplicationVersion{1, 0, 0}};
    Ely::Elysium e{engineInfo};
};