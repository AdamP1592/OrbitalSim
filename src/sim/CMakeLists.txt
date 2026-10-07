find_package(fftw3f CONFIG REQUIRED)
#message(STATUS "SIM SRC DIR= ${CMAKE_CURRENT_SOURCE_DIR}")
add_library(orbital_core
    OrbitalSim.cpp
    SimCTX.hpp
    physics/gravity/MassMesh.cpp
    physics/gravity/GridCTX.cpp
)
#set a root directory here
target_include_directories(orbital_core PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}
)
#link math to sim since sim uses math extensively
target_link_libraries(
    orbital_core PUBLIC math
)
target_link_libraries(
    orbital_core PRIVATE ParticleStorage
)
target_link_libraries(
    orbital_core PRIVATE fftw3f
)