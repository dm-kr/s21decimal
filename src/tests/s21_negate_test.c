#include "../s21_decimal.h"
#include "s21_decimal_test.h"

static s21_decimal array[] = {
    {{0x00000000, 0x00000000, 0x00000000, 0x00000000}},  // 0
    {{0x00000001, 0x00000000, 0x00000000, 0x00000000}},  // 1
    {{0x00000001, 0x00000000, 0x00000000, 0x80000000}},  // -1
    {{0xCF5C048C, 0x004D2BEF, 0x00000000, 0x00070000}},  // 2172188218.2812812
    {{0xFEADBDA2, 0x000001FB, 0x00000000, 0x80060000}},  // -2181821.218210
    {{0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF,
      0x00000000}},  // 79228162514264337593543950335
    {{0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF,
      0x80000000}},  // -79228162514264337593543950335
    {{0x00000000, 0x00000000, 0x00000000, 0x00020000}},  // 0.00
    {{0x00000000, 0x00000000, 0x00000000, 0x80020000}},  // 0.00
    {{0x000F4240, 0x00000000, 0x00000000, 0x00020000}},  // 10000.00
    {{0x000F4240, 0x00000000, 0x00000000, 0x80020000}},  // -10000.00
    {{0x00000467, 0x0000007A, 0x00000000, 0x00040000}},  // 52398601.1239
    {{0x00000467, 0x0000007A, 0x00000000, 0x80000000}},  // -523986011239
    {{0x00000467, 0x0000007A, 0x00003215,
      0x00010000}},  // 23650570576955414727997.5
    {{0x00000467, 0x0000007A, 0x00205FF0,
      0x80020000}},  // -391386782621189641642978.31
    {{0x000F4240, 0x00002710, 0x00002710,
      0x80050000}},  // -1844674407800451901.20000

};

static s21_decimal result[] = {
    {{0x00000000, 0x00000000, 0x00000000, 0x80000000}},  // 0
    {{0x00000001, 0x00000000, 0x00000000, 0x80000000}},  //-1
    {{0x00000001, 0x00000000, 0x00000000, 0x00000000}},  // 1
    {{0xCF5C048C, 0x004D2BEF, 0x00000000, 0x80070000}},  //-2172188218.2812812
    {{0xFEADBDA2, 0x000001FB, 0x00000000, 0x00060000}},  // 2181821.218210
    {{0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF,
      0x80000000}},  //-79228162514264337593543950335
    {{0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF,
      0x00000000}},  // 79228162514264337593543950335
    {{0x00000000, 0x00000000, 0x00000000, 0x80020000}},  // 0.00
    {{0x00000000, 0x00000000, 0x00000000, 0x00020000}},  // 0.00
    {{0x000F4240, 0x00000000, 0x00000000, 0x80020000}},  //-10000.00
    {{0x000F4240, 0x00000000, 0x00000000, 0x00020000}},  // 10000.00
    {{0x00000467, 0x0000007A, 0x00000000, 0x80040000}},  //-52398601.1239
    {{0x00000467, 0x0000007A, 0x00000000, 0x00000000}},  // 523986011239
    {{0x00000467, 0x0000007A, 0x00003215,
      0x80010000}},  //-23650570576955414727997.5
    {{0x00000467, 0x0000007A, 0x00205FF0,
      0x00020000}},  // 391386782621189641642978.31
    {{0x000F4240, 0x00002710, 0x00002710,
      0x00050000}},  // 1844674407800451901.20000

};

static s21_decimal error_array[] = {
    {{0x00000467, 0x0000007A, 0x00003215, 0x80010000}},
    {{0x00000008, 0x00000000, 0x00000000, 0x81000100}},
    {{0x00000008, 0x00000000, 0x00000000, 0xFFFFFFFF}},
};

START_TEST(test_negate) {
  s21_decimal tmp = {{0, 0, 0, 0}};
  int code = s21_negate(array[_i], &tmp);
  ck_assert_int_eq(s21_is_equal(tmp, result[_i]), 1);
  ck_assert_int_eq(code, 0);
}
END_TEST

START_TEST(test_negate_with_error) {
  s21_decimal empty = {{0, 0, 0, 0}};
  s21_decimal *tmp = _i == 0 ? NULL : &empty;
  int code = s21_negate(error_array[_i], tmp);
  ck_assert_int_eq(code, 1);
}
END_TEST

Suite *suite_negate(void) {
  Suite *s;
  TCase *tc_core;

  s = suite_create("NEGATE");
  tc_core = tcase_create("Core");

  tcase_add_loop_test(tc_core, test_negate, 0,
                      sizeof(array) / sizeof(s21_decimal));
  tcase_add_loop_test(tc_core, test_negate_with_error, 0,
                      sizeof(error_array) / sizeof(s21_decimal));
  suite_add_tcase(s, tc_core);

  return s;
}
