cmake_minimum_required(VERSION 3.10..3.16)

project(coronan_lib 
    VERSION 0.1 
    LANGUAGES CXX
)

set(HEADER_LIST "${CMAKE_CURRENT_SOURCE_DIR}/../include/coronan/http_client.hpp")

add_library(${PROJECT_NAME} INTERFACE)

add_library(coronan::lib ALIAS ${PROJECT_NAME})

target_include_directories(${PROJECT_NAME}
    INTERFACE
        ${CMAKE_CURRENT_SOURCE_DIR}/../include
)

target_sources(${PROJECT_NAME}
    INTERFACE 
        ${HEADER_LIST}
)

target_link_libraries(${PROJECT_NAME}
    INTERFACE CONAN_PKG::poco
)



# IDE stuff
source_group(TREE "${CMAKE_CURRENT_SOURCE_DIR}/../include" PREFIX "Header Files" FILES ${HEADER_LIST})