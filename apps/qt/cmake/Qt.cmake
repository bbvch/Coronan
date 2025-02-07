include(FetchContent)

set(FETCHCONTENT_QUIET FALSE)

# Only build the following QT submodules:
set(QT_BUILD_SUBMODULES
    "qtbase;qtcharts"
    CACHE STRING "Submodules to build"
)
set(QT_VERSION
    "v6.8.2"
    CACHE STRING "Qt6 git version"
)
set(QT_WILL_BUILD_TOOLS ON)

# Disable some unnecessary QT features to reduce the build time.
set(QT_FEATURE_sql OFF)
set(QT_FEATURE_network OFF)
set(QT_FEATURE_dbus OFF)
set(QT_FEATURE_testlib OFF)
set(QT_FEATURE_opengl OFF)
set(QT_FEATURE_opengles2 OFF)
set(QT_FEATURE_opengles3 OFF)
set(QT_FEATURE_charts ON)
set(QT_BUILD_STANDALONE_TESTS OFF)
set(QT_BUILD_EXAMPLES OFF)
set(QT_BUILD_TESTS OFF)

fetchcontent_declare(
    qt6
    GIT_REPOSITORY "https://github.com/qt/qt5.git"
    GIT_TAG "${QT_VERSION}"
    GIT_SHALLOW TRUE
    GIT_PROGRESS TRUE
    GIT_SUBMODULES ${QT_BUILD_SUBMODULES}
)

fetchcontent_makeavailable(qt6)
