configure_file("version.h.in" "${CMAKE_CURRENT_BINARY_DIR}/generated/coronan/version.h" ESCAPE_QUOTES)

add_library(coronan-version INTERFACE)

add_library(coronan::version ALIAS coronan-version)

target_include_directories(coronan-version INTERFACE ${CMAKE_CURRENT_BINARY_DIR}/generated)
