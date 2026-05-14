#include <stdint.h>
#include <stdio.h>

extern long asm_add(long a, long b);
extern int asm_sum_array(const int *items, long count);

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
  int slot = 0;

  printf("pure .S asm_add(20, 22)          = %ld\n", asm_add(20, 22));
  printf("pure .S asm_sum_array(items, 5)  = %d\n", asm_sum_array(items, 5));
  printf("inline asm add(7, 8)             = %ld\n", inline_add(7, 8));
  printf("inline asm +r add_in_place       = %ld\n",
         inline_add_in_place(100, 23));
  printf("inline asm read sp               = 0x%lx\n",
         (unsigned long)inline_read_sp());
  printf("memory barrier store/load        = %d\n",
         store_then_load_with_barrier(&slot, 99));

  return 0;
}
