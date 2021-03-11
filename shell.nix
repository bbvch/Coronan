{ pkgs ? import <nixpkgs> { } }:
with pkgs;

let
  gcc = gcc10;
  clang = clang_11;
  my-python-packages = python-packages:
    with python-packages; [
      conan
      cmake-format
    ];
  python-with-my-packages = python3.withPackages my-python-packages;

in mkShell {
  hardeningDisable = [ "all" ];
  buildInputs = [
    gcc
    ninja
    clang
    clang-tools
    dpkg
    cmake
    ccache
    qt5.full
    libGLU
    python-with-my-packages
    lcov
    perl
    doxygen
    graphviz
  ];
}
