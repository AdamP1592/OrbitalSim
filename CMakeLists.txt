# META
cmake_minimum_required(VERSION 3.25)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

project(OrbitalSim VERSION 0.0.1 LANGUAGES C CXX)

# SETUP

enable_testing()

add_subdirectory(src)
add_subdirectory(tests)

add_test(NAME MathTests COMMAND test_math)
#add_executable(orbital_sim main.cpp)
#target_link_libraries(orbital_sim PRIVATE orbital_core)