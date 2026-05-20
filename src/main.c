#include <inttypes.h>
#include <stdint.h>
#include <stdio.h>

extern int64_t asm_add(int64_t a, int64_t b);
extern int32_t asm_sum_array(const int32_t *items, int64_t count);
extern int64_t asm_sub(int64_t a, int64_t b);
extern int64_t asm_mul(int64_t a, int64_t b);
extern int64_t asm_divmod(int64_t a, int64_t b, int64_t *remainder);
extern int64_t asm_bit_mix(int64_t value, int64_t shamt);
extern int64_t asm_max(int64_t a, int64_t b);
extern int64_t asm_abs(int64_t value);
extern int64_t asm_countdown_sum(int64_t n);
extern int64_t asm_find_int(const int32_t *items, int64_t count,
                            int32_t target);
extern void asm_memset32(uint32_t *items, int64_t count, uint32_t value);
extern int64_t asm_strlen(const char *text);
extern void asm_swap(int64_t *left, int64_t *right);
extern int64_t asm_factorial(int64_t n);
extern int64_t asm_call_add_twice(int64_t a, int64_t b, int64_t c);
extern float asm_fadd(float a, float b);
extern float asm_fmuladd(float a, float b, float c);
extern double asm_ddiv(double a, double b);
extern double asm_dmax(double a, double b);
extern int64_t asm_double_to_i64(double value);

static inline int64_t inline_add(int64_t a, int64_t b) {
  int64_t out;

  __asm__ __volatile__("add %0, %1, %2" : "=r"(out) : "r"(a), "r"(b));

  return out;
}

static inline int64_t inline_add_in_place(int64_t value, int64_t delta) {
  __asm__ __volatile__("add %0, %0, %1" : "+r"(value) : "r"(delta));

  return value;
}

static inline uint64_t inline_read_sp(void) {
  uint64_t sp;

  __asm__ __volatile__("mv %0, sp" : "=r"(sp));

  return sp;
}

static inline void compiler_memory_barrier(void) {
  __asm__ __volatile__("" ::: "memory");
}

static int32_t store_then_load_with_barrier(int32_t *slot, int32_t value) {
  *slot = value;
  compiler_memory_barrier();
  return *slot;
}

int main(void) {
  int32_t items[] = {1, 2, 3, 4, 5};
  uint32_t words[4] = {0};
  int32_t slot = 0;
  int64_t remainder = 0;
  int64_t left = 11;
  int64_t right = 99;

  printf("pure .S asm_add(20, 22)          = %" PRId64 "\n", asm_add(20, 22));
  printf("pure .S asm_sum_array(items, 5)  = %" PRId32 "\n",
         asm_sum_array(items, 5));
  printf("arith asm_sub(20, 7)             = %" PRId64 "\n", asm_sub(20, 7));
  printf("arith asm_mul(6, 7)              = %" PRId64 "\n", asm_mul(6, 7));
  printf("arith asm_divmod(43, 5)          = %" PRId64 "",
         asm_divmod(43, 5, &remainder));
  printf(" remainder %" PRId64 "\n", remainder);
  printf("arith asm_bit_mix(0x1234, 4)     = 0x%" PRIx64 "\n",
         (uint64_t)asm_bit_mix(0x1234, 4));
  printf("branch asm_max(-3, 12)           = %" PRId64 "\n", asm_max(-3, 12));
  printf("branch asm_abs(-42)              = %" PRId64 "\n", asm_abs(-42));
  printf("branch asm_countdown_sum(10)     = %" PRId64 "\n",
         asm_countdown_sum(10));
  printf("memory asm_find_int(items,5,4)   = %" PRId64 "\n",
         asm_find_int(items, 5, 4));
  asm_memset32(words, 4, 0xfeedfaceu);
  printf("memory asm_memset32 words[2]     = 0x%" PRIx32 "\n", words[2]);
  printf("memory asm_strlen(\"riscv\")       = %" PRId64 "\n",
         asm_strlen("riscv"));
  asm_swap(&left, &right);
  printf("stack asm_swap(&left,&right)     = %" PRId64 " %" PRId64 "\n", left,
         right);
  printf("stack asm_factorial(6)           = %" PRId64 "\n", asm_factorial(6));
  printf("stack asm_call_add_twice(1,2,3)  = %" PRId64 "\n",
         asm_call_add_twice(1, 2, 3));
  printf("float asm_fadd(1.25, 2.5)        = %.2f\n",
         (double)asm_fadd(1.25f, 2.5f));
  printf("float asm_fmuladd(2, 3.5, 1.25)  = %.2f\n",
         (double)asm_fmuladd(2.0f, 3.5f, 1.25f));
  printf("float asm_ddiv(22, 7)            = %.12f\n", asm_ddiv(22.0, 7.0));
  printf("float asm_dmax(-3, 9.5)          = %.2f\n", asm_dmax(-3.0, 9.5));
  printf("float asm_double_to_i64(-12.75)  = %" PRId64 "\n",
         asm_double_to_i64(-12.75));
  printf("inline asm add(7, 8)             = %" PRId64 "\n", inline_add(7, 8));
  printf("inline asm +r add_in_place       = %" PRId64 "\n",
         inline_add_in_place(100, 23));
  printf("inline asm read sp               = 0x%" PRIx64 "\n",
         inline_read_sp());
  printf("memory barrier store/load        = %" PRId32 "\n",
         store_then_load_with_barrier(&slot, 99));

  return 0;
}
