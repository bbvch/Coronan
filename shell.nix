with (import ./sources.nix);
with nixpkgs;
let
  gcc = gcc10;
  clang = clang_11;
  cmake-rc = (import ./cmake.nix);

in stdenvNoCC.mkDerivation {
  name = "shell";
  hardeningDisable = [ "all" ];
  LOCALE_ARCHIVE_2_27 = "${glibcLocales}/lib/locale/locale-archive";
  buildInputs = [
    gcc
    cmake-rc.cmake
    cmake # required otherwise cmake can not find GL/gl.h. Important: must be after cmake-rc.cmake
    glibcLocales
    ninja
    clang
    binutils
    clang-tools
    include-what-you-use
    cppcheck
    dpkg
    ccache
    qt514.full
    libGLU
    lcov
    perl
    doxygen
    graphviz
    pkg-config
    pre-commit
    python3
    python3Packages.setuptools
    python3Packages.pip
    python3Packages.virtualenv
  ];

  shellHook = ''
    pre-commit install -f --hook-type pre-commit
    virtualenv venv
    source venv/bin/activate
    pip install conan==1.35.0
    pip install cmake-format==0.6.13
    pip install yamlfmt==1.1.0
  '';
}
