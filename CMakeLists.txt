cmake_minimum_required(VERSION 4.0.1)
project(cmake_gen)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

add_executable(cmake_gen
      src/cmake_gen.cpp src/cmake_gen.h src/main.cpp
)
