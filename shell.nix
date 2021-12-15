with (import ./sources.nix);
with nixpkgs;
let
  gcc = gcc10;
  clang = clang_11;
  cmake_321 = (import ./cmake.nix).cmake;

  gcovr = python3.withPackages(ps: [
    ps.gcovr
  ]);

  sphinx-env = python3.withPackages(ps: [
    ps.sphinx
    ps.sphinx_rtd_theme
    ps.breathe
  ]);

  full-sphinx-env = buildEnv {
  name = "full-sphinx-env";
  paths = [
    sphinx-env
  ] ++ (lib.optional withPdf latex);
};

in stdenvNoCC.mkDerivation {
  name = "shell";
  hardeningDisable = [ "all" ];
  LOCALE_ARCHIVE_2_27 = "${glibcLocales}/lib/locale/locale-archive";
  buildInputs = [
    gcc
    cmake_321
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
    qt514.full
    lcov
    gcovr
    perl
    doxygen
    graphviz
    pkg-config
    pre-commit
    python3
    sphinx-env
    cmake-format
    python3Packages.setuptools
    python3Packages.pip
    python3Packages.virtualenv
  ];

  shellHook = ''
    pre-commit install -f --hook-type pre-commit
    virtualenv venv
    source venv/bin/activate
    python3 -m pip install conan>=1.43
    python3 -m pip install yamlfmt>=1.1.0
    python3 -m pip install m2r2>=0.3.2
  '';
}
