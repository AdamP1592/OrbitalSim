add_subdirectory(Utils)

add_subdirectory(math)
add_subdirectory(datastructures)
add_subdirectory(storage)


add_test(NAME MathTests COMMAND test_math)
add_test(NAME FreeListTests COMMAND test_freelist)
add_test(NAME ECSStorageTests COMMAND test_storage)