add_library(StorageTestUtils
    StorageTestUtils.hpp
)
target_link_libraries(
    StorageTestUtils
    PUBLIC ParticleStorage
    PUBLIC orbital_core
)
