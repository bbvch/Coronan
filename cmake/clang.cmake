# Check for clang compiler
if(CMAKE_CXX_COMPILER MATCHES ".*clang")
    set(CMAKE_COMPILER_IS_CLANGCXX 1)
    message(STATUS "Clang found")
endif()

find_program(
    CLANG_TIDY_EXE
    NAMES clang-tidy clang-tidy-3.9 clang-tidy-3.8
    DOC "Path to clang-tidy executable"
)

if(NOT CLANG_TIDY_EXE)
    message(STATUS "clang-tidy not found.")
else()
    message(STATUS "clang-tidy found: ${CLANG_TIDY_EXE}")
    set(CLANG_TIDY_PROPERTIES "${CLANG_TIDY_EXE}" "-checks=*")
endif()