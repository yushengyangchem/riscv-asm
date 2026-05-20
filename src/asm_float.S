.text

// Floating-point examples using the RISC-V hard-float calling convention.
// float/double arguments arrive in fa0-fa7, and float/double results return in fa0.

.globl asm_fadd
.type  asm_fadd, @function

asm_fadd: // extern float asm_fadd(float a, float b);
	fadd.s fa0, fa0, fa1
	ret
.size asm_fadd, .-asm_fadd

.globl asm_fmuladd
.type  asm_fmuladd, @function

asm_fmuladd: // extern float asm_fmuladd(float a, float b, float c);
	fmadd.s fa0, fa0, fa1, fa2
	ret
.size asm_fmuladd, .-asm_fmuladd

.globl asm_ddiv
.type  asm_ddiv, @function

asm_ddiv: // extern double asm_ddiv(double a, double b);
	fdiv.d fa0, fa0, fa1
	ret
.size asm_ddiv, .-asm_ddiv

.globl asm_dmax
.type  asm_dmax, @function

asm_dmax: // extern double asm_dmax(double a, double b);
	fmax.d fa0, fa0, fa1
	ret
.size asm_dmax, .-asm_dmax

.globl asm_double_to_i64
.type  asm_double_to_i64, @function

asm_double_to_i64: // extern int64_t asm_double_to_i64(double value);
	fcvt.l.d a0, fa0, rtz
	ret
.size asm_double_to_i64, .-asm_double_to_i64
