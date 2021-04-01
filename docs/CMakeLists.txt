set(DOXYGEN_USE_MDFILE_AS_MAINPAGE "${CMAKE_CURRENT_SOURCE_DIR}/mainpage.md")

doxygen_add_docs(
  docs
  coronan/http_client.hpp
  coronan/corona-api_parser.hpp
  coronan/ssl_initializer.hpp
  "${CMAKE_CURRENT_SOURCE_DIR}/mainpage.md"
  WORKING_DIRECTORY
  "${PROJECT_SOURCE_DIR}/include"
)

include(GNUInstallDirs)

install(DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/html
        DESTINATION ${CMAKE_INSTALL_DOCDIR}
)
