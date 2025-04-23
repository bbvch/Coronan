include(GNUInstallDirs)
include(Doxygen)

enable_doxygen("awesome-sidebar")

install(
    DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/html
    DESTINATION ${CMAKE_INSTALL_DOCDIR}
    COMPONENT Coronan_Development
)
