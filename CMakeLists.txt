cmake_minimum_required(VERSION 3.15)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

project(logger)

# Explicitly opt in to modern CMake behaviors to avoid warnings
cmake_policy(SET CMP0135 NEW)

# Enable strict handling of warnings
add_definitions("-Wall -Wextra")

add_library(
  Logger src/log/level.cpp src/log/levelfilter.cpp src/log/decoration.cpp
         src/log/greeter.cpp src/log/logger.cpp src/log/loggerbuilder.cpp)

target_include_directories(Logger PUBLIC include)

# #------------------ Log demo ------------------# #
add_executable(log_demo log_demo.cpp)

target_link_libraries(log_demo Logger)

# #------------------ Logger example tfg ------------------# #
add_executable(logger_example_tfg logger_example_tfg.cpp)

target_link_libraries(logger_example_tfg Logger)

# ------------------- TEST ------------------- #
include(FetchContent)
FetchContent_Declare(
  googletest
  URL https://github.com/google/googletest/releases/download/v1.17.0/googletest-1.17.0.tar.gz 
)

set(gtest_force_shared_crt
    ON
    CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

enable_testing()

# #------------------ Log test ------------------# #
add_executable(log_test test/log_test.cpp)

target_include_directories(log_test PUBLIC test/mocks)

target_link_libraries(log_test GTest::gtest_main GTest::gmock Logger)

include(GoogleTest)
gtest_discover_tests(log_test)
