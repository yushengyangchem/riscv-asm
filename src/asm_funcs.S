.text
.globl asm_add
.type  asm_add, @function

asm_add: // extern int64_t asm_add(int64_t a, int64_t b);
	add a0, a0, a1
	ret
.size asm_add, .-asm_add

.globl asm_sum_array
.type  asm_sum_array, @function

asm_sum_array: // extern int32_t asm_sum_array(const int32_t *items, int64_t count);
	li   t0, 0
	beqz a1, 2f

1:
	lw   t1, 0(a0)
	addw t0, t0, t1
	addi a0, a0, 4
	addi a1, a1, -1
	bnez a1, 1b

2:
	mv a0, t0
	ret
.size asm_sum_array, .-asm_sum_array
