add_library(
    math INTERFACE # swap to private if it's not needed
)
target_include_directories(
    math INTERFACE
    ${CMAKE_CURRENT_SOURCE_DIR}/src/math
)
