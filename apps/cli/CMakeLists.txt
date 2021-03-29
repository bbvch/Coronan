cmake_minimum_required(VERSION 3.15...3.20)

project(
  coronan_cli
  VERSION 0.1.0
  LANGUAGES CXX
)

add_executable(${PROJECT_NAME} ${CMAKE_CURRENT_LIST_DIR}/main.cpp)

add_executable(coronan::${PROJECT_NAME} ALIAS ${PROJECT_NAME})
set_target_properties(
  ${PROJECT_NAME} PROPERTIES CXX_EXTENSIONS OFF OUTPUT_NAME coronan
)
find_package(lyra REQUIRED)

target_link_libraries(
  ${PROJECT_NAME}
  PRIVATE coronan::coronan_lib
  PRIVATE lyra::lyra
  PRIVATE coronan::compile_warnings
  PRIVATE coronan::compile_options
)

install(
  TARGETS coronan_cli
  LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
  ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
  RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)
