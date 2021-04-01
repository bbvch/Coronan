cmake_minimum_required(VERSION 3.15...3.20)

project(
  Coronan
  VERSION 0.2.0
  DESCRIPTION "Conan example project showing Covid-19 cases."
  LANGUAGES CXX
)

option(ENABLE_TESTING "Enable Test Builds" ON)

list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_LIST_DIR}/cmake")

include(cmake/StandardProjectSettings.cmake)
include(cmake/PreventInSourceBuilds.cmake)

# Link this 'library' to set the c++ standard / compile-time options requested
add_library(project_options INTERFACE)
add_library(coronan::compile_options ALIAS project_options)
target_compile_features(project_options INTERFACE cxx_std_17)

if(CMAKE_CXX_COMPILER_ID MATCHES ".*Clang")
  option(ENABLE_BUILD_WITH_TIME_TRACE
         "Enable -ftime-trace to generate time tracing .json files on clang"
         OFF
  )
  if(ENABLE_BUILD_WITH_TIME_TRACE)
    target_compile_options(project_options INTERFACE -ftime-trace)
  endif()
endif()

# Link this 'library' to use the warnings specified in CompilerWarnings.cmake
add_library(project_warnings INTERFACE)
add_library(coronan::compile_warnings ALIAS project_warnings)

# enable cache system
include(cmake/Cache.cmake)

# standard compiler warnings
include(cmake/CompilerWarnings.cmake)
set_project_warnings(project_warnings)

# sanitizer options if supported by compiler
include(cmake/Sanitizers.cmake)
enable_sanitizers(project_options)

if(CMAKE_PROJECT_NAME STREQUAL PROJECT_NAME)
  include(cmake/Doxygen.cmake)
  enable_doxygen()

  if(ENABLE_DOXYGEN)
    set_property(GLOBAL PROPERTY USE_FOLDERS ON)
    add_subdirectory(docs)
  endif()
endif()

# Very basic PCH example
option(ENABLE_PCH "Enable Precompiled Headers" OFF)
if(ENABLE_PCH)
  # This sets a global PCH parameter, each project will build its own PCH, which
  # is a good idea if any #define's change
  #
  target_precompile_headers(
    project_options INTERFACE <vector> <string> <map> <utility>
  )
endif()

include(cmake/Conan.cmake)
run_conan()

# enable coverage if supported by compiler
include(cmake/Coverage.cmake)
enable_coverage(project_options)

add_subdirectory(src)

if(ENABLE_TESTING)
  enable_testing()
  add_subdirectory(tests)
endif()

add_subdirectory(apps/cli)
add_subdirectory(apps/qt)

include(CMakePackageConfigHelpers)
include(GNUInstallDirs)

configure_package_config_file(
  ${PROJECT_SOURCE_DIR}/cmake/Coronan.cmake.in ${PROJECT_NAME}Config.cmake
  INSTALL_DESTINATION ${LIB_INSTALL_DIR}/${PROJECT_NAME}/cmake
)

write_basic_package_version_file(
  ${PROJECT_NAME}ConfigVersion.cmake
  VERSION ${PACKAGE_VERSION}
  COMPATIBILITY AnyNewerVersion
)

install(FILES ${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}Config.cmake
              ${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}ConfigVersion.cmake
        DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/${PROJECT_NAME}
)

install(
  TARGETS coronan project_warnings project_options
  EXPORT ${PROJECT_NAME}Targets
  LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
  ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
  RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)

install(
  EXPORT ${PROJECT_NAME}Targets
  FILE ${PROJECT_NAME}Targets.cmake
  NAMESPACE coronan::
  DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/${PROJECT_NAME}
)

install(DIRECTORY ${PROJECT_SOURCE_DIR}/include/
        DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
)

install(FILES LICENSE DESTINATION ${CMAKE_INSTALL_DOCDIR})

include(cmake/CPackConfig.cmake)
