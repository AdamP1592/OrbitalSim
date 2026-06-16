add_executable(test_freelist
    TestFreeList.cpp
)

target_link_libraries(
    test_freelist
    PRIVATE DataStructures
    PRIVATE LogHelper
)
