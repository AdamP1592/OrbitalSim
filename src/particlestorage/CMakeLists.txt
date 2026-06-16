add_library(ParticleStorage
    /ParticleStorage.cpp
    /ParticleView.cpp
    /components/Moveable.cpp
    /components/Position.cpp
)

target_include_directories(ParticleStorage PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}
)

target_link_libraries(ParticleStorage PUBLIC
    DataStructures
)