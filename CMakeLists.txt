cmake_minimum_required(VERSION 3.10)
project(gencmake)

set(CMAKE_CXX_STANDARD 17)

add_executable(gencmake 
    src/main.cpp
    src/cmake_gen.cpp
)

install(TARGETS gencmake DESTINATION bin)
install(FILES template/cmake_template.txt DESTINATION share/gencmake/templates)
