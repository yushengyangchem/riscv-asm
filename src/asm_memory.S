.text

// Memory examples: word loads/stores, byte loads, and pointer increments.

.globl asm_find_int
.type  asm_find_int, @function

asm_find_int: // extern int64_t asm_find_int(const int32_t *items, int64_t count, int32_t target);
	li   t0, 0
	blez a1, 2f

1:
	lw   t1, 0(a0)
	beq  t1, a2, 3f
	addi a0, a0, 4
	addi t0, t0, 1
	blt  t0, a1, 1b

2:
	li a0, -1
	ret

3:
	mv a0, t0
	ret
.size asm_find_int, .-asm_find_int

.globl asm_memset32
.type  asm_memset32, @function

asm_memset32: // extern void asm_memset32(uint32_t *items, int64_t count, uint32_t value);
	blez a1, 2f

1:
	sw   a2, 0(a0)
	addi a0, a0, 4
	addi a1, a1, -1
	bnez a1, 1b

2:
	ret
.size asm_memset32, .-asm_memset32

.globl asm_strlen
.type  asm_strlen, @function

asm_strlen: // extern int64_t asm_strlen(const char *text);
	li t0, 0

1:
	// lbu zero-extends one byte, which is what C strings need.
	lbu  t1, 0(a0)
	beqz t1, 2f
	addi a0, a0, 1
	addi t0, t0, 1
	j    1b

2:
	mv a0, t0
	ret
.size asm_strlen, .-asm_strlen
