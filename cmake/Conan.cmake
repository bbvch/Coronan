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
      "https://github.com/conan-io/cmake-conan/raw/release/0.18/conan.cmake"
      "${CMAKE_BINARY_DIR}/conan.cmake"
      EXPECTED_HASH
        SHA256=5cdb3042632da3efff558924eecefd580a0e786863a857ca097c3d1d43df5dcd
      TLS_VERIFY ON)
  endif()

  include(${CMAKE_BINARY_DIR}/conan.cmake)

  conan_cmake_configure(
    REQUIRES
    poco/1.12.4
    rapidjson/cci.20220822
    lyra/1.6.1
    fmt/9.1.0
    catch2/3.2.1
    GENERATORS
    cmake_find_package_multi
    OPTIONS
    poco:enable_mongodb=False
    poco:enable_activerecord=False
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

      conan_cmake_install(PATH_OR_REFERENCE . BUILD ${CONAN_BUILD} SETTINGS
                          ${settings})
    endforeach()
  else()
    conan_cmake_autodetect(settings)

    conan_cmake_install(PATH_OR_REFERENCE . BUILD ${CONAN_BUILD} SETTINGS
                        ${settings})
  endif()

  list(APPEND CMAKE_MODULE_PATH ${CMAKE_BINARY_DIR})
  list(APPEND CMAKE_PREFIX_PATH ${CMAKE_BINARY_DIR})
endmacro()
