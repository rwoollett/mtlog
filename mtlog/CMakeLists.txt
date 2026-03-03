
add_library(MTLog
    STATIC src/mt_log.cpp
)

target_include_directories(MTLog
  PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/include      # <mtlog/mt_log.h>
)

target_link_libraries(MTLog PUBLIC fmt::fmt)
target_compile_features(MTLog PUBLIC cxx_std_20)

