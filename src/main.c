#include <stdint.h>
#include <stdio.h>

extern long asm_add(long a, long b);
extern int asm_sum_array(const int *items, long count);
extern long asm_sub(long a, long b);
extern long asm_mul(long a, long b);
extern long asm_divmod(long a, long b, long *remainder);
extern long asm_bit_mix(long value, long shamt);
extern long asm_max(long a, long b);
extern long asm_abs(long value);
extern long asm_countdown_sum(long n);
extern long asm_find_int(const int *items, long count, int target);
extern void asm_memset32(uint32_t *items, long count, uint32_t value);
extern long asm_strlen(const char *text);
extern void asm_swap(long *left, long *right);
extern long asm_factorial(long n);
extern long asm_call_add_twice(long a, long b, long c);

static inline long inline_add(long a, long b) {
  long out;

  __asm__ __volatile__("add %0, %1, %2" : "=r"(out) : "r"(a), "r"(b));

  return out;
}

static inline long inline_add_in_place(long value, long delta) {
  __asm__ __volatile__("add %0, %0, %1" : "+r"(value) : "r"(delta));

  return value;
}

static inline uintptr_t inline_read_sp(void) {
  uintptr_t sp;

  __asm__ __volatile__("mv %0, sp" : "=r"(sp));

  return sp;
}

static inline void compiler_memory_barrier(void) {
  __asm__ __volatile__("" ::: "memory");
}

static int store_then_load_with_barrier(int *slot, int value) {
  *slot = value;
  compiler_memory_barrier();
  return *slot;
}

int main(void) {
  int items[] = {1, 2, 3, 4, 5};
  uint32_t words[4] = {0};
  int slot = 0;
  long remainder = 0;
  long left = 11;
  long right = 99;

  printf("pure .S asm_add(20, 22)          = %ld\n", asm_add(20, 22));
  printf("pure .S asm_sum_array(items, 5)  = %d\n", asm_sum_array(items, 5));
  printf("arith asm_sub(20, 7)             = %ld\n", asm_sub(20, 7));
  printf("arith asm_mul(6, 7)              = %ld\n", asm_mul(6, 7));
  printf("arith asm_divmod(43, 5)          = %ld",
         asm_divmod(43, 5, &remainder));
  printf(" remainder %ld\n", remainder);
  printf("arith asm_bit_mix(0x1234, 4)     = 0x%lx\n",
         (unsigned long)asm_bit_mix(0x1234, 4));
  printf("branch asm_max(-3, 12)           = %ld\n", asm_max(-3, 12));
  printf("branch asm_abs(-42)              = %ld\n", asm_abs(-42));
  printf("branch asm_countdown_sum(10)     = %ld\n", asm_countdown_sum(10));
  printf("memory asm_find_int(items,5,4)   = %ld\n", asm_find_int(items, 5, 4));
  asm_memset32(words, 4, 0xfeedfaceu);
  printf("memory asm_memset32 words[2]     = 0x%x\n", words[2]);
  printf("memory asm_strlen(\"riscv\")       = %ld\n", asm_strlen("riscv"));
  asm_swap(&left, &right);
  printf("stack asm_swap(&left,&right)     = %ld %ld\n", left, right);
  printf("stack asm_factorial(6)           = %ld\n", asm_factorial(6));
  printf("stack asm_call_add_twice(1,2,3)  = %ld\n",
         asm_call_add_twice(1, 2, 3));
  printf("inline asm add(7, 8)             = %ld\n", inline_add(7, 8));
  printf("inline asm +r add_in_place       = %ld\n",
         inline_add_in_place(100, 23));
  printf("inline asm read sp               = 0x%lx\n",
         (unsigned long)inline_read_sp());
  printf("memory barrier store/load        = %d\n",
         store_then_load_with_barrier(&slot, 99));

  return 0;
}
