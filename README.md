# RISC-V ASM Lab

This project is a small lab for learning two things:

1. How standalone RISC-V assembly files (`.S`) call into C and are called from C.
2. How GCC inline assembly works in C with `__asm__ __volatile__`, constraints, and clobbers.

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

## Test

```sh
make test
```

The tests build a separate RISC-V executable from `src/test_asm.c` and run it through QEMU. They cover the standalone assembly examples with a few normal and edge-case inputs.

## Inspect Assembly

```sh
make dump
```

You can also ask the compiler to generate assembly for a single C file:

```sh
riscv64-unknown-linux-gnu-gcc -std=c11 -O2 -S -o build/main.s src/main.c
```

## Assembly Examples

The standalone assembly examples are split by topic:

- `src/asm_funcs.S`: first minimal examples, including a scalar add and an array sum.
- `src/asm_arith.S`: arithmetic, multiply/divide/remainder, shifts, and bitwise ops.
- `src/asm_branch.S`: signed branches, simple selection, and counted loops.
- `src/asm_memory.S`: `lw`, `sw`, `lbu`, pointer increments, and sentinel loops.
- `src/asm_stack.S`: memory through pointers, a leaf loop, and a non-leaf function that saves `ra`/`s0` on the stack before calling another assembly function.
- `src/asm_float.S`: single- and double-precision floating-point arithmetic, fused multiply-add, max, and conversion to integer.

Useful ABI reminders while reading the files:

- Arguments and return values use `a0`-`a7`. Most examples return through `a0`.
- Floating-point arguments and return values use `fa0`-`fa7` when compiling with the hard-float ABI.
- Temporaries `t0`-`t6` are caller-saved, so tiny leaf functions can freely use them.
- Saved registers `s0`-`s11` must be restored by the callee when used.
- Non-leaf functions that call another function must preserve `ra`, usually on the stack.
