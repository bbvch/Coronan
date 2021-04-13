cmake_minimum_required(VERSION 3.15...3.20)

project(
  coronan_tests
  VERSION 0.1
  LANGUAGES CXX)

if(CONAN_CATCH2_ROOT_DEBUG)
  include(${CONAN_CATCH2_ROOT_DEBUG}/lib/cmake/Catch2/Catch.cmake)
else()
  include(${CONAN_CATCH2_ROOT}/lib/cmake/Catch2/Catch.cmake)
endif()

add_executable(unittests ${CMAKE_CURRENT_LIST_DIR}/main.cpp)

add_executable(coronan::UnitTest ALIAS unittests)

target_sources(
  unittests PRIVATE ${CMAKE_CURRENT_LIST_DIR}/http_client_test.cpp
                    ${CMAKE_CURRENT_LIST_DIR}/corona-api_json_parser_test.cpp)

target_link_libraries(
  unittests
  PRIVATE coronan::coronan_lib
  PRIVATE CONAN_PKG::catch2
  PRIVATE coronan::compile_warnings
  PRIVATE coronan::compile_options)

catch_discover_tests(
  unittests
  TEST_PREFIX
  "unittest."
  REPORTER
  xml
  OUTPUT_DIR
  .
  OUTPUT_PREFIX
  "unittest."
  OUTPUT_SUFFIX
  .xml)

add_custom_target(run_unittests ALL COMMAND $<TARGET_FILE:coronan::UnitTest>)
