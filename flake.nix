{
  description = "Lost at C dev environment";

  inputs = {
    nixpkgs.url = "github:NixOS/nixpkgs/nixos-26.05";
  };

  outputs = { self, nixpkgs }:
    let
      system = "x86_64-linux";
      pkgs = import nixpkgs { inherit system; config = {}; overlays = []; };
    in
    {
      devShells.${system}.default = pkgs.mkShell {
        name = "weather-balloon-environment";
        buildInputs = with pkgs; [
          gcc
          gnumake
          python313
          clang-tools
        ];
        shellHook = ''
        export CPATH="${pkgs.glibc.dev}/include:$CPATH"
        '';
      };
    };
}
