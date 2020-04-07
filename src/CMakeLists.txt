cmake_minimum_required(VERSION 3.10..3.16)

project(
    coronan_lib
    VERSION 0.1
    LANGUAGES CXX
)

set(HEADER_LIST
    "${CMAKE_CURRENT_SOURCE_DIR}/../include/coronan/http_client.hpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/../include/coronan/corona-api_parser.hpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/../include/coronan/ssl_initializer.hpp"
)

add_library(${PROJECT_NAME} STATIC ${HEADER_LIST})

add_library(coronan::lib ALIAS ${PROJECT_NAME})

target_include_directories(
    ${PROJECT_NAME} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/../include
)

target_sources(${PROJECT_NAME} 
    PRIVATE 
        ${CMAKE_CURRENT_SOURCE_DIR}/corona-api_parser.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/ssl_initializer.cpp
        ${HEADER_LIST}
)

target_link_libraries(
    ${PROJECT_NAME}
    PUBLIC CONAN_PKG::poco
    PUBLIC CONAN_PKG::rapidjson
)

# IDE stuff
source_group(
    TREE "${CMAKE_CURRENT_SOURCE_DIR}/../include"
    PREFIX "Header Files"
    FILES ${HEADER_LIST}
)
