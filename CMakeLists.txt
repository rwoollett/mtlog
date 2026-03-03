cmake_minimum_required(VERSION 3.22)
project(MTLogger VERSION 1.0
        DESCRIPTION "Dedicated logging thread + queue"
        LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
include(FetchContent)

# Fetch fmt automatically 
FetchContent_Declare( 
  fmt 
  GIT_REPOSITORY https://github.com/fmtlib/fmt.git 
  GIT_TAG 10.2.1 
) 
FetchContent_MakeAvailable(fmt)

add_subdirectory(mtlog)

# Add an option to control whether to build tests
option(BUILD_MTLOG_TESTS "Build tests and include Google Test" OFF)
if (BUILD_MTLOG_TESTS)
  FetchContent_Declare(
    googletest
    GIT_REPOSITORY https://github.com/google/googletest.git
    GIT_TAG        v1.15.2
  )
  FetchContent_MakeAvailable(googletest)
  include(CTest) # dashboard
  #enable_testing()
  add_subdirectory(tests)
endif()




