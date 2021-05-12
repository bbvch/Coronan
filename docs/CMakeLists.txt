set(DOXYGEN_GENERATE_XML YES)
set(DOXYGEN_GENERATE_HTML NO)

doxygen_add_docs(
  doxygen
  coronan/http_client.hpp
  coronan/corona-api_parser.hpp
  coronan/corona-api_client.hpp
  coronan/ssl_client.hpp
  WORKING_DIRECTORY
  "${PROJECT_SOURCE_DIR}/include")

find_package(Sphinx REQUIRED)

set(SPHINX_SOURCE ${CMAKE_CURRENT_SOURCE_DIR})
set(SPHINX_BUILD ${CMAKE_CURRENT_BINARY_DIR}/html)

add_custom_target(
  docs
  COMMAND
    ${SPHINX_EXECUTABLE} -b html
    # Tell Breathe where to find the Doxygen output
    -Dbreathe_projects.Coronan=${CMAKE_CURRENT_BINARY_DIR}/xml ${SPHINX_SOURCE}
    ${SPHINX_BUILD}
  WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
  DEPENDS doxygen
  COMMENT "Generating documentation with Sphinx")

include(GNUInstallDirs)

install(
  DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/html
  DESTINATION ${CMAKE_INSTALL_DOCDIR}
  COMPONENT Coronan_Development)
