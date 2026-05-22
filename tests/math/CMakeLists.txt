add_executable(test_math
    TestVec3.cpp
)

target_link_libraries(
    test_math
    PRIVATE math
    PRIVATE LogHelper
)
