cmake_minimum_required(VERSION 3.10)

project(covidan_tests VERSION 0.1 LANGUAGES CXX)

add_executable(${PROJECT_NAME}
    ${CMAKE_CURRENT_LIST_DIR}/main.cpp
)

target_link_libraries(${PROJECT_NAME}
    PRIVATE covidnan::lib
    PRIVATE CONAN_PKG::doctest
)


add_test(NAME covidnantest COMMAND ${PROJECT_NAME})