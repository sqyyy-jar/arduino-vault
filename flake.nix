{
  inputs = {
    nixpkgs.url = "github:NixOS/nixpkgs/nixos-unstable";
    flake-utils.url = "github:numtide/flake-utils";
  };
  outputs = { self, nixpkgs, flake-utils }:
    flake-utils.lib.eachDefaultSystem
      (system:
        let
          pkgs = import nixpkgs {
            inherit system;
          };
        in
        with pkgs;
        {
          devShells.default = mkShell {
            buildInputs = [
              arduino-cli
              arduino-language-server
              avrdude
              pkgsCross.avr.buildPackages.gcc
              clang
              pkg-config
              libudev-zero
              gnumake
            ];
          };
        }
      );
}
