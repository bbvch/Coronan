cmake_minimum_required(VERSION 3.10)

project(
    coronan_qt
    VERSION 0.1
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

add_executable(coronan::App ALIAS ${PROJECT_NAME})

find_package(
    Qt5 HINTS /home/komax/Qt/5.14.2/gcc_64
    COMPONENTS Charts
    REQUIRED
)
find_package(Qt5Core CONFIG REQUIRED)

target_link_libraries(
    ${PROJECT_NAME}
    PRIVATE coronan::lib
    PRIVATE Qt5::Charts
)

target_compile_options(
    ${PROJECT_NAME}
    PRIVATE -Wall
            -Wextra
            -Werror
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
            -Wno-narrowing
            -Wstrict-overflow=5
            -Wswitch-default
            -Wundef
)

if(CMAKE_COMPILER_IS_GNUCXX)
    target_compile_options(
        ${PROJECT_NAME} PRIVATE -Wlogical-op -Wnoexcept -Wstrict-null-sentinel
    )
elseif(CMAKE_COMPILER_IS_CLANGCXX)
    target_compile_options(
        ${PROJECT_NAME} PRIVATE -pedantic
                                -Wno-gnu-zero-variadic-macro-arguments
    )

endif(CMAKE_COMPILER_IS_GNUCXX)

target_compile_options(
    ${PROJECT_NAME} PUBLIC "$<$<BOOL:MSVC>:-D_WIN32_WINNT=0x0601>"
                           "$<$<BOOL:UNIX>:-fPIC;>"
)

if(CLANG_TIDY_EXE AND NOT CMAKE_COMPILER_IS_GNUCXX)
    set_target_properties(
        ${PROJECT_NAME} PROPERTIES CXX_CLANG_TIDY "${CLANG_TIDY_PROPERTIES}"
    )
endif()
