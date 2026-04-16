cmake_minimum_required(VERSION 3.16)

add_executable(TestMTLog 
  main.cpp
  testMTLog.cpp
)

target_link_libraries(TestMTLog gtest_main gmock_main MTLog)
include(GoogleTest)
gtest_discover_tests(TestMTLog)

