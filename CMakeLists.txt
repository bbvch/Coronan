cmake_minimum_required(VERSION 3.10)

project(
    Coronan
    VERSION 0.1
    DESCRIPTION "Conan example project showing Covid-19 cases."
    LANGUAGES CXX
)

option(BUILD_TESTING "Build the testing tree." ON)
option(CODE_COVERAGE "Enable coverage reporting" OFF)

if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Debug)
endif()

list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_LIST_DIR}/cmake")
include(clang)

include(conan)

conan_cmake_run(CONANFILE conanfile.txt BASIC_SETUP CMAKE_TARGETS BUILD poco missing)

if(CMAKE_PROJECT_NAME STREQUAL PROJECT_NAME)

    set_property(GLOBAL PROPERTY USE_FOLDERS ON)

    find_package(Doxygen)
    if(Doxygen_FOUND)
        add_subdirectory(docs)
    else()
        message(STATUS "Doxygen not found. Docu is not built")
    endif()
endif()

message(STATUS "CMAKE_PREFIX_PATH: ${CMAKE_PREFIX_PATH}")

add_library(coverage_config INTERFACE)
add_library(code_coverage::config ALIAS coverage_config)

if(CODE_COVERAGE AND CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")
    # Add required flags (GCC & LLVM/Clang)
    target_compile_options(
        coverage_config
        INTERFACE -O0 # no optimization
                  -g # generate debug info
                  --coverage # sets all required flags
    )
    if(CMAKE_VERSION VERSION_GREATER_EQUAL 3.13)
        target_link_options(coverage_config INTERFACE --coverage)
    else()
        target_link_libraries(coverage_config INTERFACE --coverage)
    endif()
endif()

add_subdirectory(src)

# Only build tests if we are the top-level project
if(BUILD_TESTING AND (PROJECT_SOURCE_DIR STREQUAL CMAKE_SOURCE_DIR))
    include(CTest)
    add_subdirectory(tests)
endif()

add_subdirectory(apps/cli)
add_subdirectory(apps/qt)
