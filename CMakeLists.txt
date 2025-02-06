cmake_minimum_required(VERSION 3.15...3.31)

project(
    Coronan
    VERSION 0.2.0
    DESCRIPTION "Conan example project showing Covid-19 cases."
    LANGUAGES CXX
)

option(ENABLE_TESTING "Enable Test Builds" ON)

# conan cmake_layout puts files used for find_package into specific place. Added
# it to CMAKE_PREFIX_PATH
if(CMAKE_CONFIGURATION_TYPES)
    # Multi-configuration generator (e.g. Visual Studio)
    list(APPEND CMAKE_PREFIX_PATH "${CMAKE_CURRENT_BINARY_DIR}/generators")
else()
    # Single-configuration generator
    list(APPEND CMAKE_PREFIX_PATH
         "${CMAKE_CURRENT_BINARY_DIR}/${CMAKE_BUILD_TYPE}/generators"
    )
endif()

list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_LIST_DIR}/cmake")

include(cmake/StandardProjectSettings.cmake)
include(cmake/PreventInSourceBuilds.cmake)

# Link this 'library' to set the c++ standard / compile-time options requested
add_library(project_options INTERFACE)
add_library(coronan::compile_options ALIAS project_options)
target_compile_features(project_options INTERFACE cxx_std_20)

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

    if(ENABLE_CLANG_TIDY)
        message(
            WARNING
                "Using ENABLE_CLANG_TIDY together with ENABLE_PCH will give compiler errors."
        )
    endif(ENABLE_CLANG_TIDY)
    # This sets a global PCH parameter, each project will build its own PCH,
    # which is a good idea if any #define's change
    #
    target_precompile_headers(project_options INTERFACE <vector> <string>)
endif()

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
    NO_SET_AND_CHECK_MACRO NO_CHECK_REQUIRED_COMPONENTS_MACRO
)

write_basic_package_version_file(
    ${PROJECT_NAME}ConfigVersion.cmake
    VERSION ${PACKAGE_VERSION}
    COMPATIBILITY AnyNewerVersion
)

if(NOT WIN32 AND CMAKE_SOURCE_DIR STREQUAL CMAKE_CURRENT_SOURCE_DIR)
    set(CMAKE_INSTALL_PREFIX "/opt/bbv.ch/${PROJECT_NAME}")
endif()

install(
    FILES ${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}Config.cmake
          ${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}ConfigVersion.cmake
    DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/${PROJECT_NAME}
    COMPONENT Coronan_Development
)

install(
    DIRECTORY ${PROJECT_SOURCE_DIR}/include/
    DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
    COMPONENT Coronan_Development
)

install(
    FILES LICENSE
    DESTINATION ${CMAKE_INSTALL_DOCDIR}
    COMPONENT Coronan_Development
)

if(CMAKE_SOURCE_DIR STREQUAL CMAKE_CURRENT_SOURCE_DIR)
    include(cmake/CPackConfig.cmake)
endif()
