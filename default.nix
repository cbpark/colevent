{ pkgs ? (import <nixpkgs> {}), source ? ./., version ? "dev" }:

with pkgs;

stdenv.mkDerivation {
  name = "colevent-${version}";
  src = lib.cleanSource source;
  enableParallelBuilding = true;
  buildInputs = [ root ];

  configureFlags = "--with-rootsys=${root}";

  meta = {};
}
