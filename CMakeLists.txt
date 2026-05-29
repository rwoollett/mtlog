cmake_minimum_required(VERSION 3.22)
project(MTLogger VERSION 1.0
        DESCRIPTION "Dedicated logging thread + queue"
        LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

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

if (BUILD_MTLOG_TESTS)
    include(FetchContent)
    FetchContent_Declare(
        googletest
        GIT_REPOSITORY https://github.com/google/googletest.git
        GIT_TAG v1.15.2
    )
    FetchContent_MakeAvailable(googletest)

    include(CTest)
    enable_testing()
    add_subdirectory(tests)
endif()
