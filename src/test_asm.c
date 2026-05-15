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

static int failures;

static void expect_i64(const char *name, int64_t actual, int64_t expected) {
  if (actual == expected) {
    return;
  }

  printf("FAIL %-32s got %" PRId64 ", expected %" PRId64 "\n", name, actual,
         expected);
  failures++;
}

static void expect_u64(const char *name, uint64_t actual, uint64_t expected) {
  if (actual == expected) {
    return;
  }

  printf("FAIL %-32s got 0x%" PRIx64 ", expected 0x%" PRIx64 "\n", name, actual,
         expected);
  failures++;
}

static void expect_u32(const char *name, uint32_t actual, uint32_t expected) {
  if (actual == expected) {
    return;
  }

  printf("FAIL %-32s got 0x%" PRIx32 ", expected 0x%" PRIx32 "\n", name, actual,
         expected);
  failures++;
}

static void test_basic_funcs(void) {
  const int32_t items[] = {1, 2, 3, 4, 5};
  const int32_t negatives[] = {-10, 20, -30, 40};

  expect_i64("asm_add positive", asm_add(20, 22), 42);
  expect_i64("asm_add negative", asm_add(-5, 12), 7);
  expect_i64("asm_sum_array full", asm_sum_array(items, 5), 15);
  expect_i64("asm_sum_array empty", asm_sum_array(items, 0), 0);
  expect_i64("asm_sum_array mixed", asm_sum_array(negatives, 4), 20);
}

static void test_arith(void) {
  int64_t remainder = 0;

  expect_i64("asm_sub", asm_sub(20, 7), 13);
  expect_i64("asm_mul", asm_mul(-6, 7), -42);

  expect_i64("asm_divmod quotient", asm_divmod(43, 5, &remainder), 8);
  expect_i64("asm_divmod remainder", remainder, 3);

  remainder = 1234;
  expect_i64("asm_divmod divide by zero", asm_divmod(43, 0, &remainder), 0);
  expect_i64("asm_divmod zero remainder", remainder, 43);

  expect_u64("asm_bit_mix shift 4", (uint64_t)asm_bit_mix(0x1234, 4),
             UINT64_C(0x12277));
}

static void test_branch(void) {
  expect_i64("asm_max left", asm_max(12, -3), 12);
  expect_i64("asm_max right", asm_max(-3, 12), 12);
  expect_i64("asm_max equal", asm_max(7, 7), 7);
  expect_i64("asm_abs positive", asm_abs(42), 42);
  expect_i64("asm_abs negative", asm_abs(-42), 42);
  expect_i64("asm_abs zero", asm_abs(0), 0);
  expect_i64("asm_countdown_sum ten", asm_countdown_sum(10), 55);
  expect_i64("asm_countdown_sum one", asm_countdown_sum(1), 1);
  expect_i64("asm_countdown_sum zero", asm_countdown_sum(0), 0);
  expect_i64("asm_countdown_sum negative", asm_countdown_sum(-5), 0);
}

static void test_memory(void) {
  const int32_t items[] = {1, 2, 3, 4, 5};
  uint32_t words[] = {1, 2, 3, 4};

  expect_i64("asm_find_int middle", asm_find_int(items, 5, 4), 3);
  expect_i64("asm_find_int first", asm_find_int(items, 5, 1), 0);
  expect_i64("asm_find_int missing", asm_find_int(items, 5, 99), -1);
  expect_i64("asm_find_int empty", asm_find_int(items, 0, 1), -1);

  asm_memset32(words, 4, 0xfeedfaceu);
  expect_u32("asm_memset32 first", words[0], 0xfeedfaceu);
  expect_u32("asm_memset32 last", words[3], 0xfeedfaceu);

  asm_memset32(words, 0, 0);
  expect_u32("asm_memset32 zero count", words[0], 0xfeedfaceu);

  expect_i64("asm_strlen text", asm_strlen("riscv"), 5);
  expect_i64("asm_strlen empty", asm_strlen(""), 0);
}

static void test_stack(void) {
  int64_t left = 11;
  int64_t right = 99;

  asm_swap(&left, &right);
  expect_i64("asm_swap left", left, 99);
  expect_i64("asm_swap right", right, 11);

  expect_i64("asm_factorial six", asm_factorial(6), 720);
  expect_i64("asm_factorial one", asm_factorial(1), 1);
  expect_i64("asm_factorial zero", asm_factorial(0), 1);
  expect_i64("asm_call_add_twice", asm_call_add_twice(1, 2, 3), 6);
  expect_i64("asm_call_add_twice mixed", asm_call_add_twice(-10, 5, 2), -3);
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
