cmake_minimum_required(VERSION 3.22)

enable_testing()

project(tests)

option(gtest_disable_pthreads "Disable uses of pthreads in gtest." ON)

add_subdirectory(googletest)
include(GoogleTest)

set(tests_SRCS
    ../app/math/crc/crc8maxim.cpp
    crc8maxim-tests.cpp
)

set(tests_PATHS
    ../app/math/crc/
)

add_executable(${PROJECT_NAME} ${tests_SRCS})

target_include_directories(${PROJECT_NAME} PRIVATE ${tests_PATHS})

target_link_libraries(${PROJECT_NAME} PUBLIC gtest_main)

gtest_discover_tests(${PROJECT_NAME})