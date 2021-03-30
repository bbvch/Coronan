with (import ./inputs.nix);
with pkgs;
let
  gcc = gcc10;
  clang = clang_11;
  cmake-rc = (import ./cmake.nix);

in mkShell {
  hardeningDisable = [ "all" ];
  LOCALE_ARCHIVE_2_27 = "${glibcLocales}/lib/locale/locale-archive";
  buildInputs = [
    cmake-rc.cmake
    cmake # required otherwise cmake can not find GL/gl.h. Important: must be after cmake-rc.cmake
    mach-nix.mach-nix
    glibcLocales
    gcc
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
    pip install conan==1.34.0
    pip install cmake-format==0.6.13
    pip install yamlfmt==1.1.0
  '';
}
