cmake_minimum_required(VERSION 3.16)

add_library(MTLog STATIC
    src/mt_log.cpp
)

add_library(MTLog::MTLog ALIAS MTLog)

target_include_directories(MTLog
  PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:include>
)

target_link_libraries(MTLog
  PUBLIC
    fmt::fmt
)

target_compile_features(MTLog PUBLIC cxx_std_20)

# -----------------------------
# INSTALL RULES
# -----------------------------
install(TARGETS MTLog
    EXPORT MTLogTargets
    ARCHIVE DESTINATION lib
    LIBRARY DESTINATION lib
    RUNTIME DESTINATION bin
)

install(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/include/
    DESTINATION include
)

install(EXPORT MTLogTargets
    FILE MTLogTargets.cmake
    NAMESPACE MTLog::
    DESTINATION lib/cmake/MTLog
)
