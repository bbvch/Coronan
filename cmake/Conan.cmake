macro(run_conan)

  option(ENABLE_CONAN_BUILD_ALL
         "Enable rebuild of all conan requires from source" OFF)

  # Download automatically, you can also just copy the conan.cmake file
  if(NOT EXISTS "${CMAKE_BINARY_DIR}/conan.cmake")
    message(
      STATUS
        "Downloading conan.cmake from https://github.com/conan-io/cmake-conan")
    file(
      DOWNLOAD
      "https://raw.githubusercontent.com/conan-io/cmake-conan/v0.16.1/conan.cmake"
      "${CMAKE_BINARY_DIR}/conan.cmake"
      EXPECTED_HASH
        SHA256=396e16d0f5eabdc6a14afddbcfff62a54a7ee75c6da23f32f7a31bc85db23484
      TLS_VERIFY ON)
  endif()

  include(${CMAKE_BINARY_DIR}/conan.cmake)

  conan_cmake_configure(
    REQUIRES
    poco/1.10.1
    rapidjson/1.1.0
    lyra/1.5.1
    fmt/7.1.3
    catch2/2.13.4
    GENERATORS
    cmake_find_package_multi
    OPTIONS
    poco:enable_mongodb=False
    poco:enable_data=False
    poco:enable_data_sqlite=False
    poco:enable_data_mysql=False
    poco:enable_data_postgresql=False)

  if(ENABLE_CONAN_BUILD_ALL)
    set(CONAN_BUILD all)
  else()
    set(CONAN_BUILD missing)
  endif()

  if(CMAKE_CONFIGURATION_TYPES)
    foreach(TYPE ${CMAKE_CONFIGURATION_TYPES})
      conan_cmake_autodetect(settings BUILD_TYPE ${TYPE})

      conan_cmake_install(
        PATH_OR_REFERENCE
        .
        BUILD
        ${CONAN_BUILD}
        REMOTE
        conancenter
        SETTINGS
        ${settings})
    endforeach()
  else()
    conan_cmake_autodetect(settings)

    conan_cmake_install(
      PATH_OR_REFERENCE
      .
      BUILD
      ${CONAN_BUILD}
      REMOTE
      conancenter
      SETTINGS
      ${settings})
  endif()

  list(APPEND CMAKE_MODULE_PATH ${CMAKE_BINARY_DIR})
  list(APPEND CMAKE_PREFIX_PATH ${CMAKE_BINARY_DIR})
endmacro()
