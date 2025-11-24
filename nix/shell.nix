with (import ./sources.nix);
with nixpkgs;
let
  gcc = gcc14;
  clang = clang_18;
  cmake_331 = (import ./cmake.nix).cmake;

in stdenvNoCC.mkDerivation {
  name = "shell";
  hardeningDisable = [ "all" ];
  LOCALE_ARCHIVE_2_27 = "${glibcLocales}/lib/locale/locale-archive";
  buildInputs = [
    gcc
    cmake_331
    cmake
    glibcLocales
    ninja
    clang
    binutils
    clang-tools
    include-what-you-use
    cppcheck
    dpkg
    ccache
    libGLU
    libGL
    qt6.qtbase
    qt6.qtcharts
    lcov
    perl
    doxygen
    graphviz
    pkg-config
    pre-commit
    python3
    cmake-format
    gcovr
    python3Packages.setuptools
    python3Packages.pip
    python3Packages.virtualenv
  ];

  shellHook = ''
    pre-commit install -f --hook-type pre-commit
    virtualenv venv
    venv/bin/python3 -m pip install conan
    venv/bin/python3 -m pip install python-dateutil
  '';
}
