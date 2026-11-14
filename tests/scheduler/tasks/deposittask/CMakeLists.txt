add_executable(test_deposit
    DepositTests.cpp
)
add_executable(test_deposit_reduction
    DepositReductionTests.cpp
)
add_executable(test_deposit_reproducable_reduction
    DepositReproducableReductionTests.cpp
)
add_executable(test_deposit
    DepositTests.cpp
)
target_link_libraries(
    test_math
    PRIVATE math
    PRIVATE LogHelper
    PRIVATE ParticleStorage
    PRIVATE TaskTestUtils
)
