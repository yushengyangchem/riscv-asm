.text

// Basic integer ALU examples: sub, mul, div/rem, shifts, and bitwise ops.

.globl asm_sub
.type  asm_sub, @function

asm_sub: // extern int64_t asm_sub(int64_t a, int64_t b);
	sub a0, a0, a1
	ret
.size asm_sub, .-asm_sub

.globl asm_mul
.type  asm_mul, @function

asm_mul: // extern int64_t asm_mul(int64_t a, int64_t b);
	mul a0, a0, a1
	ret
.size asm_mul, .-asm_mul

.globl asm_divmod
.type  asm_divmod, @function

asm_divmod: // extern int64_t asm_divmod(int64_t a, int64_t b, int64_t *remainder);
	beqz a1, 1f
	rem  t0, a0, a1
	div  a0, a0, a1
	sd   t0, 0(a2)
	ret

1:
	sd a0, 0(a2)
	li a0, 0
	ret
.size asm_divmod, .-asm_divmod

.globl asm_bit_mix
.type  asm_bit_mix, @function

asm_bit_mix: // extern int64_t asm_bit_mix(int64_t value, int64_t shamt);
	// RISC-V RV64 variable shifts use the low 6 bits of the shift amount.
	andi a1, a1, 63
	sll  t0, a0, a1
	srl  t1, a0, a1
	xor  t0, t0, t1
	andi t1, a0, 255
	or   a0, t0, t1
	ret
.size asm_bit_mix, .-asm_bit_mix
