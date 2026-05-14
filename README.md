# RISC-V ASM Lab

This project is a small lab for learning two things:

1. How standalone RISC-V assembly files (`.S`) call into C and are called from C.
2. How GCC inline assembly works in C with `__asm__ __volatile__`, constraints, and clobbers.

> Note: the correct spelling is `__volatile__`, not `__volatie__`.

## Enter the Environment

```sh
direnv allow
```

Or:

```sh
nix-shell
```

## Build and Run

```sh
make
make run
```

`make run` uses `qemu-riscv64` to run the cross-compiled RISC-V Linux program, so you do not need a physical RISC-V machine.

## Inspect Assembly

```sh
make dump
```

You can also ask the compiler to generate assembly for a single C file:

```sh
riscv64-unknown-linux-gnu-gcc -std=c11 -O2 -S -o build/main.s src/main.c
```
