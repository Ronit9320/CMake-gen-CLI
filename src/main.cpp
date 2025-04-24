#include "cmake_gen.h"
#include <iostream>

int main(int argc, char *argv[]) {
  try {
    cmake_gen();
    return 0;
  } catch (const std::exception &e) {
    std::cerr << "Error: " << e.what() << std::endl;
    return 1;
  } catch (...) {
    std::cerr << "Unknown error occurred" << std::endl;
    return 1;
  }
}
