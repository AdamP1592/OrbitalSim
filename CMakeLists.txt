cmake_minimum_required(VERSION 3.25)

project(OrbitalSim VERSION 0.0.1 LANGUAGES C C++)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(FFTW3f CONFIG REQUIRED)

add_subdirectory(src)

add_executable(orbital_sim main.cpp)
target_link_libraries(orbital_sim PRIVATE orbital_core)