.text

// Pointer mutation, a leaf loop, and one non-leaf function with a stack frame.

.globl asm_swap
.type  asm_swap, @function

asm_swap:
	ld t0, 0(a0)
	ld t1, 0(a1)
	sd t1, 0(a0)
	sd t0, 0(a1)
	ret
.size asm_swap, .-asm_swap

.globl asm_factorial
.type  asm_factorial, @function

asm_factorial:
	li   t0, 1
	blez a0, 2f

1:
	mul  t0, t0, a0
	addi a0, a0, -1
	bnez a0, 1b

2:
	mv a0, t0
	ret
.size asm_factorial, .-asm_factorial

.globl asm_call_add_twice
.type  asm_call_add_twice, @function

asm_call_add_twice:
	// Save callee-saved state before calling another function.
	addi sp, sp, -16
	sd   ra, 8(sp)
	sd   s0, 0(sp)

	mv   s0, a2
	call asm_add
	mv   a1, s0
	call asm_add

	ld   s0, 0(sp)
	ld   ra, 8(sp)
	addi sp, sp, 16
	ret
.size asm_call_add_twice, .-asm_call_add_twice
