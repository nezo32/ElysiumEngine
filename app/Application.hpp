#pragma once

#include "elysium/ely_device.h"
#include "elysium/ely_events.h"
#include "elysium/ely_window.h"

class Application {
public:
  void Run();

private:
  static constexpr int WIDTH = 1280;
  static constexpr int HEIGHT = 720;
  static constexpr const char *APP_NAME = "ZALUPA";

  Ely::ElyWindow elyWindow{WIDTH, HEIGHT, APP_NAME};
  Ely::ElyDevice elyDevice{elyWindow};
};