cmake_minimum_required(VERSION 3.15)
project(queue_macros LANGUAGES C)
add_library(queue_macros INTERFACE)
target_include_directories(queue_macros INTERFACE include)
option(QUEUE_MACROS_BUILD_TESTS "Build the queue-macros tests" ON)

if(QUEUE_MACROS_BUILD_TESTS)
    add_subdirectory(tests)
endif()
