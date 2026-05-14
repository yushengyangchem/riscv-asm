{
  pkgs ? import <nixpkgs> { },
}:

pkgs.mkShell {
  packages = with pkgs; [
    qemu
    gdb
    clang-tools
    pkgsCross.riscv64.stdenv.cc
    bear
    nixfmt
    prettier
    asmfmt
  ];
}
