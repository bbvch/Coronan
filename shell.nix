with (import ./sources.nix);
with nixpkgs;
let
  gcc = gcc10;
  clang = clang_11;
  cmake_320 = (import ./cmake.nix).cmake;

in stdenvNoCC.mkDerivation {
  name = "shell";
  hardeningDisable = [ "all" ];
  LOCALE_ARCHIVE_2_27 = "${glibcLocales}/lib/locale/locale-archive";
  buildInputs = [
    gcc
    cmake_320
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
    qt5.qtbase
    qt5.qtcharts
    lcov
    perl
    doxygen
    graphviz
    pkg-config
    pre-commit
    python3
    cmake-format
    python3Packages.setuptools
    python3Packages.pip
    python3Packages.virtualenv
  ];

  shellHook = ''
    pre-commit install -f --hook-type pre-commit
    virtualenv venv
    source venv/bin/activate
    python3 -m pip install conan==1.35.0
    python3 -m pip install yamlfmt==1.1.0
  '';
}
