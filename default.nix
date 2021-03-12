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
      cmake-format>=0.6.13
    '';
  };

cmake-rc = stdenv.mkDerivation {
  pname = "cmake";
  version = "3.20.0-rc4";

  src = fetchurl {
    url = "https://github.com/Kitware/CMake/releases/download/v3.20.0-rc4/cmake-3.20.0-rc4.tar.gz";
    sha256 = "34cda5a38a627e799658d8bb15982701ca5e542e112d88396d42596a03037c7c";
  };

  buildInputs = [
    openssl
    cmake
    ccache
    ninja
  ];

};

in mkShell {
  hardeningDisable = [ "all" ];
  LOCALE_ARCHIVE_2_27 = "${glibcLocales}/lib/locale/locale-archive";
  buildInputs = [
    conan-python
    glibcLocales
    cmake-format-python
    gcc
    ninja
    clang
    clang-tools
    dpkg
    cmake-rc
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
