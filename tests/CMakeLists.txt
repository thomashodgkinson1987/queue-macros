add_executable(run_tests
    main.c
    int_queue.c
    int_queue.h
)

target_link_libraries(run_tests PRIVATE queue_macros)
