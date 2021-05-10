cmake_minimum_required(VERSION 3.15...3.20)

project(
  coronan_qt
  VERSION 0.2.0
  LANGUAGES CXX)

set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

if(DEFINED ENV{Qt5_DIR} AND EXISTS "$ENV{Qt5_DIR}/lib")
  set(CMAKE_LIBRARY_PATH ${CMAKE_LIBRARY_PATH} "$ENV{Qt5_DIR}/lib")
endif()

set(CMAKE_BUILD_WITH_INSTALL_RPATH false)
set(CMAKE_INSTALL_RPATH_USE_LINK_PATH false)
set(CMAKE_INSTALL_RPATH ${CMAKE_LIBRARY_PATH})

message(STATUS "CMAKE_LIBRARY_PATH ${CMAKE_LIBRARY_PATH}")

add_executable(${PROJECT_NAME} ${CMAKE_CURRENT_LIST_DIR}/main.cpp)

add_executable(coronan::${PROJECT_NAME} ALIAS ${PROJECT_NAME})
set_target_properties(${PROJECT_NAME} PROPERTIES CXX_EXTENSIONS OFF OUTPUT_NAME
                                                                    coronanApp)
target_include_directories(
  ${PROJECT_NAME} PUBLIC $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
                         $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>)

set(HEADER_LIST
    "${CMAKE_CURRENT_SOURCE_DIR}/include/country_overview_table.hpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/include/country_chart_view.hpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/include/mainwindow.h")

target_sources(
  ${PROJECT_NAME}
  PRIVATE ${CMAKE_CURRENT_LIST_DIR}/mainwindow.cpp
          ${CMAKE_CURRENT_LIST_DIR}/mainwindow.ui
          ${CMAKE_CURRENT_LIST_DIR}/country_overview_table.cpp
          ${CMAKE_CURRENT_LIST_DIR}/country_chart_view.cpp
          ${HEADER_LIST})

find_package(
  Qt5
  COMPONENTS Charts
  REQUIRED)
find_package(Qt5Core CONFIG REQUIRED)

target_link_libraries(
  ${PROJECT_NAME}
  PRIVATE coronan::coronan_lib
  PRIVATE Qt5::Charts
  PRIVATE coronan::compile_warnings
  PRIVATE coronan::compile_options)

include(StaticAnalyzers)
# enable_static_analysis(${PROJECT_NAME})

include(CMakePackageConfigHelpers)
include(GNUInstallDirs)

install(
  TARGETS coronan_qt
  LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
  ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
  RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR})
