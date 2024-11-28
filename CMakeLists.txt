cmake_minimum_required(VERSION 3.5) 

project(my_project LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)

include(FetchContent)

FetchContent_Declare(
    googletest
    GIT_REPOSITORY https://github.com/google/googletest
    GIT_TAG HEAD
)
FetchContent_MakeAvailable(googletest)

add_executable(
    main
    main.cpp
    is_prime.hpp
)

enable_testing()

add_executable(
    is_prime_tests
    ./tests/is_prime_tests.cpp
)
target_link_libraries(
    is_prime_tests
    GTest::gtest_main
)

include(GoogleTest)
gtest_discover_tests(is_prime_tests)