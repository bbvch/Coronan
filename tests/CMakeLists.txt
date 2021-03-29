cmake_minimum_required(VERSION 3.15...3.20)

project(
  coronan_tests
  VERSION 0.1
  LANGUAGES CXX
)

add_executable(${PROJECT_NAME} ${CMAKE_CURRENT_LIST_DIR}/main.cpp)

add_executable(coronan::UnitTest ALIAS ${PROJECT_NAME})

target_sources(
  ${PROJECT_NAME}
  PRIVATE ${CMAKE_CURRENT_LIST_DIR}/http_client_test.cpp
          ${CMAKE_CURRENT_LIST_DIR}/corona-api_json_parser_test.cpp
)

find_package(doctest REQUIRED)

target_link_libraries(
  ${PROJECT_NAME}
  PRIVATE coronan::coronan_lib
  PRIVATE doctest::doctest
  PRIVATE coronan::compile_warnings
  PRIVATE coronan::compile_options
)

set_target_properties(${PROJECT_NAME} PROPERTIES CXX_EXTENSIONS OFF)

add_test(NAME coronan_test COMMAND $<TARGET_FILE:coronan::UnitTest>)

add_custom_target(run_unittests ALL COMMAND $<TARGET_FILE:coronan::UnitTest>)
