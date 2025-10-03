add_executable(int-queue-tests
    test_int_queue.c
    int_queue.c
)
target_link_libraries(int-queue-tests PRIVATE queue-macros)
add_test(NAME int-queue-tests COMMAND int-queue-tests)
