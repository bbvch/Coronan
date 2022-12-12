cmake_minimum_required(VERSION 3.15...3.20)

project(
  coronan_tests
  VERSION 0.1
  LANGUAGES CXX)

add_executable(
  unittests
  ${CMAKE_CURRENT_LIST_DIR}/http_client_test.cpp
  ${CMAKE_CURRENT_LIST_DIR}/corona-api_json_parser_test.cpp
  ${CMAKE_CURRENT_LIST_DIR}/corona-api_client_test.cpp)

add_executable(coronan::unittests ALIAS unittests)

find_package(Catch2 REQUIRED CONFIG)

target_link_libraries(
  unittests
  PRIVATE Catch2::Catch2WithMain
  PRIVATE coronan::library
  PRIVATE coronan::compile_warnings
  PRIVATE coronan::compile_options)

include(CTest)
include(Catch)

catch_discover_tests(
  unittests
  TEST_PREFIX
  "unittest."
  REPORTER
  XML
  OUTPUT_DIR
  .
  OUTPUT_PREFIX
  "unittest."
  OUTPUT_SUFFIX
  .xml)

add_custom_target(
  run_unittests ALL
  COMMAND $<TARGET_FILE:coronan::unittests>
  COMMENT "Run unittests")

find_package(Gcovr)
if(ENABLE_COVERAGE AND Gcovr_FOUND)
  add_custom_target(
    report_coverage ALL
    DEPENDS run_unittests
    COMMAND
      ${GCOVR_EXECUTABLE} -r ${CMAKE_SOURCE_DIR} -e ".*apps.*" -e ".*tests.*"
    WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
    COMMENT "Report coverage")
endif()
