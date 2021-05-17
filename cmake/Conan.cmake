macro(run_conan)
  # Download automatically, you can also just copy the conan.cmake file
  if(NOT EXISTS "${PROJECT_BINARY_DIR}/conan.cmake")
    message(
      STATUS
        "Downloading conan.cmake from https://github.com/conan-io/cmake-conan")
    file(DOWNLOAD
         "https://github.com/conan-io/cmake-conan/raw/v0.16.1/conan.cmake"
         "${PROJECT_BINARY_DIR}/conan.cmake")
  endif()

  include(${CMAKE_BINARY_DIR}/conan.cmake)

  option(ENABLE_CONAN_BUILD_ALL "Enable rebuild of all conan requires from source" OFF)

  if (ENABLE_CONAN_BUILD_ALL)
    conan_cmake_run(CONANFILE conanfile.txt BASIC_SETUP CMAKE_TARGETS BUILD all)
  else()
    conan_cmake_run(CONANFILE conanfile.txt BASIC_SETUP CMAKE_TARGETS BUILD missing)
  endif()
  list(APPEND CMAKE_PREFIX_PATH "${PROJECT_BINARY_DIR}")
endmacro()
