cmake_minimum_required(VERSION 3.5)
set(CMAKE_CXX_STANDARD 20)
project(zip)

find_package(Catch2 REQUIRED)
add_executable(tests test.cpp catch.hpp)
target_link_libraries(tests PRIVATE Catch2::Catch2)

include(CTest)
include(Catch)
catch_discover_tests(tests)
