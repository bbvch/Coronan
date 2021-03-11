{ pkgs ? import <nixpkgs> { } }:
with pkgs;

let
  gcc = gcc10;
  clang = clang_11;
  mach-nix = import (builtins.fetchGit {
    url = "https://github.com/DavHau/mach-nix/";
    ref = "refs/tags/3.1.1";
  }) {
    python = "python38";
  };
 
  conan-python = mach-nix.buildPythonPackage {
    src = builtins.fetchGit{
      url = "https://github.com/conan-io/conan.git";
      ref = "1.34.1";
    };
  };
 
 cmake-format-python = mach-nix.mkPython {
    requirements = ''
      cmake-format
    '';
  };

in mkShell {
  hardeningDisable = [ "all" ];
  buildInputs = [
    conan-python
    cmake-format-python
    gcc
    ninja
    clang
    clang-tools
    dpkg
    cmake
    ccache
    qt5.full
    libGLU
    lcov
    perl
    doxygen
    graphviz
  ];
}
