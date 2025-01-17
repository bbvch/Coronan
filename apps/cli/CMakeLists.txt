cmake_minimum_required(VERSION 3.15...3.20)

project(
    coronan_cli
    VERSION 0.2.0
    LANGUAGES CXX
)

add_executable(coronan_cli ${CMAKE_CURRENT_LIST_DIR}/main.cpp)

add_executable(coronan::coronan_cli ALIAS coronan_cli)
set_target_properties(
    coronan_cli PROPERTIES CXX_EXTENSIONS OFF OUTPUT_NAME coronan
)
find_package(lyra REQUIRED CONFIG)
find_package(fmt REQUIRED CONFIG)

target_link_libraries(
    coronan_cli
    PRIVATE bfg::lyra
    PRIVATE fmt::fmt
    PRIVATE coronan::library
    PRIVATE coronan::compile_warnings
    PRIVATE coronan::compile_options
)

include(StaticAnalyzers)
enable_static_analysis(coronan_cli)

install(TARGETS coronan_cli RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
                                    COMPONENT Coronan_Runtime_CLI
)
