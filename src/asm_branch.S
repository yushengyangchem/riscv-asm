.text

// Signed branches and counted loops.

.globl asm_max
.type  asm_max, @function

asm_max: // extern int64_t asm_max(int64_t a, int64_t b);
	bge a0, a1, 1f
	mv  a0, a1

1:
	ret
.size asm_max, .-asm_max

.globl asm_abs
.type  asm_abs, @function

asm_abs: // extern int64_t asm_abs(int64_t value);
	bgez a0, 1f
	neg  a0, a0

1:
	ret
.size asm_abs, .-asm_abs

.globl asm_countdown_sum
.type  asm_countdown_sum, @function

asm_countdown_sum: // extern int64_t asm_countdown_sum(int64_t n);
	li   t0, 0
	blez a0, 2f

1:
	add  t0, t0, a0
	addi a0, a0, -1
	bnez a0, 1b

2:
	mv a0, t0
	ret
.size asm_countdown_sum, .-asm_countdown_sum
