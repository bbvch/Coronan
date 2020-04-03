cmake_minimum_required(VERSION 3.10)

project(
    covidan_app
    VERSION 0.1
    LANGUAGES CXX
)

add_executable(${PROJECT_NAME} ${CMAKE_CURRENT_LIST_DIR}/main.cpp)

add_executable(coronan::App ALIAS ${PROJECT_NAME})

target_link_libraries(${PROJECT_NAME} PRIVATE coronan::lib)

target_compile_options(
    ${PROJECT_NAME}
    PRIVATE -Wall
            -Wextra
            -Werror
            -Wcast-align
            -Wconversion
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

target_compile_features(${PROJECT_NAME} PUBLIC cxx_std_17)
set_target_properties(${PROJECT_NAME} PROPERTIES CXX_EXTENSIONS OFF)
