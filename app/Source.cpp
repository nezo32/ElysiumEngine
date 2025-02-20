#include "Application.hpp"
#include <cstdlib>
#include <iostream>
#include <stdexcept>

int main() {
  try {
    Application app;
    app.Run();
  } catch (const std::exception &e) {
    std::cerr << e.what() << std::endl;
    return EXIT_FAILURE;
  }

  return EXIT_SUCCESS;
}
