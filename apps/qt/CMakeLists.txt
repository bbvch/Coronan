cmake_minimum_required(VERSION 3.22)

project(
    coronan_gui
    VERSION 0.2.0
    LANGUAGES CXX
)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

option(DOWNLOAD_QT6 "Download Qt from https://github.com/qt/qt5.git" OFF)

if(DEFINED ENV{Qt6_DIR} AND EXISTS "$ENV{Qt6_DIR}/lib")
    set(CMAKE_LIBRARY_PATH ${CMAKE_LIBRARY_PATH} "$ENV{Qt6_DIR}/lib")
endif()

if(DOWNLOAD_QT6)
    include(cmake/Qt.cmake)
else()
    set(CMAKE_AUTOMOC ON)
    set(CMAKE_AUTORCC ON)
    set(CMAKE_AUTOUIC ON)

    find_package(
        Qt6
        COMPONENTS Charts Core
        REQUIRED
    )
endif()

set(CMAKE_BUILD_WITH_INSTALL_RPATH false)
set(CMAKE_INSTALL_RPATH_USE_LINK_PATH false)

include(GNUInstallDirs)
file(RELATIVE_PATH relDir ${CMAKE_CURRENT_BINARY_DIR}/${CMAKE_INSTALL_BINDIR}
     ${CMAKE_CURRENT_BINARY_DIR}/${CMAKE_INSTALL_LIBDIR}
)

set(CMAKE_INSTALL_RPATH $ORIGIN $ORIGIN/${relDir})

message(STATUS "CMAKE_LIBRARY_PATH ${CMAKE_LIBRARY_PATH}")

add_executable(coronan_gui ${CMAKE_CURRENT_LIST_DIR}/main.cpp)

add_executable(coronan::coronan_gui ALIAS coronan_gui)
set_target_properties(
    coronan_gui PROPERTIES CXX_EXTENSIONS OFF OUTPUT_NAME coronanApp
)
target_include_directories(
    coronan_gui PUBLIC $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
                       $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>
)

set(HEADER_LIST
    "${CMAKE_CURRENT_SOURCE_DIR}/include/country_overview_table_model.hpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/include/country_data_model.hpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/include/country_chart_view.hpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/include/mainwindow.h"
)

target_sources(
    coronan_gui
    PRIVATE ${CMAKE_CURRENT_LIST_DIR}/mainwindow.cpp
            ${CMAKE_CURRENT_LIST_DIR}/country_overview_table_model.cpp
            ${CMAKE_CURRENT_LIST_DIR}/country_data_model.cpp
            ${CMAKE_CURRENT_LIST_DIR}/country_chart_view.cpp
            ${CMAKE_CURRENT_LIST_DIR}/mainwindow.ui
            ${HEADER_LIST}
)

target_link_libraries(
    coronan_gui
    PRIVATE Qt6::Core
    PRIVATE Qt6::Charts
    PRIVATE coronan::library
    PRIVATE coronan::compile_warnings
    PRIVATE coronan::compile_options
)

include(StaticAnalyzers)
# enable_static_analysis(${PROJECT_NAME})

include(CMakePackageConfigHelpers)

install(TARGETS coronan_gui RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
                                    COMPONENT Coronan_Runtime_GUI
)
