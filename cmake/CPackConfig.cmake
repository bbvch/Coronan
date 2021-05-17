# general settings
set(CPACK_PACKAGE_NAME "Coronan")
set(CPACK_PACKAGE_VENDOR "bbv Software Services AG")
set(CPACK_PACKAGE_DESCRIPTION_SUMMARY
    "Present the current data of Corona (Covid-19) cases for a country.")
set(CPACK_PACKAGE_INSTALL_DIRECTORY ${CPACK_PACKAGE_NAME})
set(CPACK_VERBATIM_VARIABLES YES)
set(CPACK_RESOURCE_FILE_README ${PROJECT_SOURCE_DIR}/README.md)

# WIX installer needs a licence file with .txt ending
configure_file(${PROJECT_SOURCE_DIR}/LICENSE  ${CMAKE_CURRENT_BINARY_DIR}/LICENSE.txt COPYONLY)
set(CPACK_RESOURCE_FILE_LICENSE ${CMAKE_CURRENT_BINARY_DIR}/LICENSE.txt)

# versions
set(CPACK_PACKAGE_VERSION_MAJOR "${PROJECT_VERSION_MAJOR}")
set(CPACK_PACKAGE_VERSION_MINOR "${PROJECT_VERSION_MINOR}")
set(CPACK_PACKAGE_VERSION_PATCH "${PROJECT_VERSION_PATCH}")

if(WIN32)
  set(CPACK_GENERATOR ZIP WIX)
elseif(APPLE)
  set(CPACK_GENERATOR TGZ productbuild)
elseif(CMAKE_SYSTEM_NAME STREQUAL "Linux")
  set(CPACK_GENERATOR TGZ)
else()
  set(CPACK_GENERATOR TGZ)
endif()

find_program(DPKG_PROGRAM dpkg)
if(DPKG_PROGRAM)
  list(APPEND CPACK_GENERATOR DEB)
  set(CPACK_DEBIAN_Coronan_Development_PACKAGE_SHLIBDEPS ON)
  # DEB package config
  set(CPACK_DEBIAN_PACKAGE_NAME "coronan")

  set(CPACK_PACKAGE_CONTACT "michel.estermann@bbv.ch")
  set(CPACK_DEBIAN_PACKAGE_MAINTAINER
      "Michel Estermann <michel.estermann@bbv.ch>")
  set(CPACK_DEBIAN_PACKAGE_HOMEPAGE "https://github.com/bbvch/Coronan")
  set(CPACK_DEBIAN_Coronan_Development_PACKAGE_DEPENDS
      "libqt5widgets5 (>=5.9.2),libQt5Charts5 (>=5.9.2)")
endif(DPKG_PROGRAM)

# IFW (Qt Installer Framework) config
set(CPACK_IFW_VERBOSE ON)
set(CPACK_IFW_PACKAGE_TITLE "Co[ro]nan")
include(CPack)
include(CPackIFW)

cpack_add_component(
  Coronan_Development
  DISPLAY_NAME
  "coronan SDK"
  DESCRIPTION
  "Development components"
  INSTALL_TYPES
  Full
  Developer)

cpack_add_component(
  Coronan_Runtime_GUI
  DISPLAY_NAME
  "coronan Qt App"
  DESCRIPTION
  "Gui executables"
  INSTALL_TYPES
  Full
  Developer)

cpack_add_component(
  Coronan_Runtime_CLI
  DISPLAY_NAME
  "coronan command line interface"
  DESCRIPTION
  "Gui executables"
  INSTALL_TYPES
  Full
  Developer
  Minimal)

cpack_add_install_type(Full)
cpack_add_install_type(Minimal)
cpack_add_install_type(Developer DISPLAY_NAME "SDK Development")
