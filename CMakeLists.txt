cmake_minimum_required(VERSION 3.15)

# Specify your project's name
project(drone_server)

# Specify at least C++11, better C++17
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Explicitly opt in to modern CMake behaviors to avoid warnings
cmake_policy(SET CMP0135 NEW)

# Enable strict handling of warnings
add_definitions("-Wall -Wextra")

add_subdirectory(lib/log)

##------------------ Log demo ------------------##
add_executable(log_demo
    log_demo.cpp
)

target_link_libraries(log_demo
    Log
)

##------------------ Logger example tfg ------------------##
add_executable(logger_example_tfg
    logger_example_tfg.cpp
)

target_link_libraries(logger_example_tfg
    Log
)

#------------------- TEST -------------------#
include(FetchContent)
FetchContent_Declare(
    googletest
    URL https://github.com/google/googletest/archive/03597a01ee50ed33e9dfd640b249b4be3799d395.zip
)

set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

enable_testing()

##------------------ Log test ------------------##
add_executable(log_test
    test/log_test.cpp
)

target_link_libraries(log_test
    GTest::gtest_main
    Log
)

include(GoogleTest)
gtest_discover_tests(log_test)