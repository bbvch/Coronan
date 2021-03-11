cmake_minimum_required(VERSION 3.10)

project(
    coronan_qt
    VERSION 0.1.0
    LANGUAGES CXX
)

set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

add_executable(
    ${PROJECT_NAME}
    ${CMAKE_CURRENT_LIST_DIR}/main.cpp ${CMAKE_CURRENT_LIST_DIR}/mainwindow.cpp
    ${CMAKE_CURRENT_LIST_DIR}/mainwindow.h
    ${CMAKE_CURRENT_LIST_DIR}/mainwindow.ui
)

add_executable(coronan::${PROJECT_NAME} ALIAS ${PROJECT_NAME})
set_target_properties(${PROJECT_NAME} PROPERTIES OUTPUT_NAME coronanApp)

find_package(
    Qt5
    COMPONENTS Charts
    REQUIRED
)
find_package(Qt5Core CONFIG REQUIRED)

target_link_libraries(
    ${PROJECT_NAME}
    PRIVATE coronan::coronan_lib
    PRIVATE Qt5::Charts
)

target_compile_options(
    ${PROJECT_NAME}
    PRIVATE -Wall
            -Werror
            -Wextra
            -pedantic
            -Wcast-align
            -Wunused
            -Wpointer-arith
            -Wcast-qual
            -Wcast-qual
            -Wctor-dtor-privacy
            -Wdisabled-optimization
            -Wformat=2
            -Winit-self
            -Wmissing-declarations
            -Wmissing-include-dirs
            -Wold-style-cast
            -Woverloaded-virtual
            -Wredundant-decls
            -Wshadow
            -Wsign-conversion
            -Wsign-promo
            -Wstrict-overflow=5
            -Wswitch-default
            -Wundef
            $<$<CONFIG:Debug>:-Og>
            $<$<CONFIG:Release>:-O3>
)

if(CMAKE_COMPILER_IS_GNUCXX)
    target_compile_options(
        ${PROJECT_NAME} PRIVATE -Wlogical-op -Wnoexcept -Wstrict-null-sentinel
                                -fconcepts
    )
elseif(CMAKE_COMPILER_IS_CLANGCXX)
    target_compile_options(
        ${PROJECT_NAME} PRIVATE -Wno-gnu-zero-variadic-macro-arguments
    )
endif(CMAKE_COMPILER_IS_GNUCXX)

if(CMAKE_BUILD_TYPE MATCHES Debug)
    target_compile_options(
        ${PROJECT_NAME} PRIVATE -fsanitize=address -fno-omit-frame-pointer
    )
    target_link_libraries(
        ${PROJECT_NAME} PUBLIC "-fno-omit-frame-pointer -fsanitize=address"
    )
endif()

target_compile_options(
    ${PROJECT_NAME} PUBLIC $<$<BOOL:MSVC>:-D_WIN32_WINNT=0x0601>
                           $<$<BOOL:UNIX>:-fPIC;>
)

install(
    TARGETS coronan_qt
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)
