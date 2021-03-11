# Check for clang compiler
if(CMAKE_CXX_COMPILER MATCHES ".*clang")
    set(CMAKE_COMPILER_IS_CLANGCXX 1)
    message(STATUS "Clang found")
endif()

find_program(
    CLANG_TIDY_EXE
    NAMES clang-tidy clang-tidy-8 clang-tidy-9 clang-tidy-10 clang-tidy-11
    DOC "Path to clang-tidy executable"
)

if(NOT CLANG_TIDY_EXE)
    message(STATUS "clang-tidy not found.")
else()
    set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
    message(STATUS "clang-tidy found: ${CLANG_TIDY_EXE}")
    set(CLANG_TIDY_PROPERTIES
        "${CLANG_TIDY_EXE}"
        "-p=${PROJECT_BINARY_DIR}"
        "-checks=-*,clang-analyzer-*,cppcoreguidelines-*,hicpp-*,modernize-*,-modernize-use-trailing-return-type,performance-*,portability-*,readability-*"
    )
endif()
