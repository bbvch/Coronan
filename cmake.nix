with builtins;
with (import ./inputs.nix);

let lock = fromTOML (readFile ./lock.toml);
in {
  cmake = pkgs.stdenv.mkDerivation {
    pname = "cmake";
    version = "${lock.cmake.rev}";

    src = fetchurl {
      url =
        "https://github.com/Kitware/CMake/releases/download/v${lock.cmake.rev}/cmake-${lock.cmake.rev}.tar.gz";
      sha256 = "${lock.cmake.sha256}";
    };

    buildInputs = [ pkgs.openssl pkgs.cmake pkgs.ccache pkgs.ninja ];

  };
}
