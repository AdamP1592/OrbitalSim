add_executable(
    test_storage
    test_storage.cpp
)

target_link_libraries(
    test_storage
    PRIVATE ParticleStorage
    PRIVATE math
    PRIVATE LogHelper
)
