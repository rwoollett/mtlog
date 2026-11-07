cmake_minimum_required(VERSION 3.22)
project(MTLogger VERSION 1.0
        DESCRIPTION "Dedicated logging thread + queue"
        LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_C_FLAGS_TSAN "-O1 -g -fsanitize=thread -fno-omit-frame-pointer")
set(CMAKE_CXX_FLAGS_TSAN "-O1 -g -fsanitize=thread -fno-omit-frame-pointer")
set(CMAKE_EXE_LINKER_FLAGS_TSAN "-fsanitize=thread")
set(CMAKE_SHARED_LINKER_FLAGS_TSAN "-fsanitize=thread")

find_package(fmt REQUIRED)

add_subdirectory(mtlog)

# -----------------------------
# PACKAGE CONFIG EXPORT
# -----------------------------
include(CMakePackageConfigHelpers)

write_basic_package_version_file(
    "${CMAKE_CURRENT_BINARY_DIR}/MTLogConfigVersion.cmake"
    VERSION ${PROJECT_VERSION}
    COMPATIBILITY SameMajorVersion
)

configure_package_config_file(
    "${CMAKE_CURRENT_SOURCE_DIR}/cmake/MTLogConfig.cmake.in"
    "${CMAKE_CURRENT_BINARY_DIR}/MTLogConfig.cmake"
    INSTALL_DESTINATION lib/cmake/MTLog
)

install(FILES
    "${CMAKE_CURRENT_BINARY_DIR}/MTLogConfig.cmake"
    "${CMAKE_CURRENT_BINARY_DIR}/MTLogConfigVersion.cmake"
    DESTINATION lib/cmake/MTLog
)

# Optional tests
option(BUILD_MTLOG_TESTS "Build tests and include Google Test" OFF)
option(BUILD_MTLOG_TSAN "Build tests for TSAN Test" OFF)

if (BUILD_MTLOG_TSAN)
    add_subdirectory(tsantest)
endif()

if (BUILD_MTLOG_TESTS)
    include(FetchContent)
    FetchContent_Declare(
        googletest
        GIT_REPOSITORY https://github.com/google/googletest.git
        GIT_TAG v1.15.2
    )
    FetchContent_MakeAvailable(googletest)

    include(CTest)
    #enable_testing()
    add_subdirectory(tests)
endif()
