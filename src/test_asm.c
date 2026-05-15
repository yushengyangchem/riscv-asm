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

static int failures;

static void expect_long(const char *name, long actual, long expected) {
  if (actual == expected) {
    return;
  }

  printf("FAIL %-32s got %ld, expected %ld\n", name, actual, expected);
  failures++;
}

static void expect_ulong(const char *name, unsigned long actual,
                         unsigned long expected) {
  if (actual == expected) {
    return;
  }

  printf("FAIL %-32s got 0x%lx, expected 0x%lx\n", name, actual, expected);
  failures++;
}

static void test_basic_funcs(void) {
  const int items[] = {1, 2, 3, 4, 5};
  const int negatives[] = {-10, 20, -30, 40};

  expect_long("asm_add positive", asm_add(20, 22), 42);
  expect_long("asm_add negative", asm_add(-5, 12), 7);
  expect_long("asm_sum_array full", asm_sum_array(items, 5), 15);
  expect_long("asm_sum_array empty", asm_sum_array(items, 0), 0);
  expect_long("asm_sum_array mixed", asm_sum_array(negatives, 4), 20);
}

static void test_arith(void) {
  long remainder = 0;

  expect_long("asm_sub", asm_sub(20, 7), 13);
  expect_long("asm_mul", asm_mul(-6, 7), -42);

  expect_long("asm_divmod quotient", asm_divmod(43, 5, &remainder), 8);
  expect_long("asm_divmod remainder", remainder, 3);

  remainder = 1234;
  expect_long("asm_divmod divide by zero", asm_divmod(43, 0, &remainder), 0);
  expect_long("asm_divmod zero remainder", remainder, 43);

  expect_ulong("asm_bit_mix shift 4", (unsigned long)asm_bit_mix(0x1234, 4),
               0x12277);
}

static void test_branch(void) {
  expect_long("asm_max left", asm_max(12, -3), 12);
  expect_long("asm_max right", asm_max(-3, 12), 12);
  expect_long("asm_max equal", asm_max(7, 7), 7);
  expect_long("asm_abs positive", asm_abs(42), 42);
  expect_long("asm_abs negative", asm_abs(-42), 42);
  expect_long("asm_abs zero", asm_abs(0), 0);
  expect_long("asm_countdown_sum ten", asm_countdown_sum(10), 55);
  expect_long("asm_countdown_sum one", asm_countdown_sum(1), 1);
  expect_long("asm_countdown_sum zero", asm_countdown_sum(0), 0);
  expect_long("asm_countdown_sum negative", asm_countdown_sum(-5), 0);
}

static void test_memory(void) {
  const int items[] = {1, 2, 3, 4, 5};
  uint32_t words[] = {1, 2, 3, 4};

  expect_long("asm_find_int middle", asm_find_int(items, 5, 4), 3);
  expect_long("asm_find_int first", asm_find_int(items, 5, 1), 0);
  expect_long("asm_find_int missing", asm_find_int(items, 5, 99), -1);
  expect_long("asm_find_int empty", asm_find_int(items, 0, 1), -1);

  asm_memset32(words, 4, 0xfeedfaceu);
  expect_ulong("asm_memset32 first", words[0], 0xfeedfaceu);
  expect_ulong("asm_memset32 last", words[3], 0xfeedfaceu);

  asm_memset32(words, 0, 0);
  expect_ulong("asm_memset32 zero count", words[0], 0xfeedfaceu);

  expect_long("asm_strlen text", asm_strlen("riscv"), 5);
  expect_long("asm_strlen empty", asm_strlen(""), 0);
}

static void test_stack(void) {
  long left = 11;
  long right = 99;

  asm_swap(&left, &right);
  expect_long("asm_swap left", left, 99);
  expect_long("asm_swap right", right, 11);

  expect_long("asm_factorial six", asm_factorial(6), 720);
  expect_long("asm_factorial one", asm_factorial(1), 1);
  expect_long("asm_factorial zero", asm_factorial(0), 1);
  expect_long("asm_call_add_twice", asm_call_add_twice(1, 2, 3), 6);
  expect_long("asm_call_add_twice mixed", asm_call_add_twice(-10, 5, 2), -3);
}

int main(void) {
  test_basic_funcs();
  test_arith();
  test_branch();
  test_memory();
  test_stack();

  if (failures != 0) {
    printf("%d assembly test(s) failed\n", failures);
    return 1;
  }

  printf("all assembly tests passed\n");
  return 0;
}
