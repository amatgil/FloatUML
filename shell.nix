{
  pkgs ? import <nixpkgs> { },
  lib,
}:
let
  packages = with pkgs; [
    gcc
    gnumake
    indent
    man-pages man-pages-posix

    cmake
    pkgs.libGL

    # X11 dependencies
    xorg.libX11
    xorg.libX11.dev
    xorg.libXcursor
    xorg.libXi
    xorg.libXinerama
    xorg.libXrandr
    emscripten # web support

    clang-tools
    valgrind
    gdb

    rust-analyzer
    rustfmt
    mold
    rust-bin.stable.latest.default

    pkg-config
    libGL
    alsa-lib
  ];
in
pkgs.mkShell {
  # Get dependencies from the main package
  nativeBuildInputs = packages;
  buildInputs = packages;
  env = {
    LIBCLANG_PATH = "${pkgs.libclang.lib}/lib";
    LD_LIBRARY_PATH = "${lib.makeLibraryPath packages}";
  };
}
