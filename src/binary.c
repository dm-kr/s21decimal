#include "binary.h"

#include "handlers.h"
#include "s21_decimal.h"
#include "utility.h"

int get_bit(s21_decimal value, int index) {
  return (value.bits[index / INT_SIZE] & (1 << (index % INT_SIZE))) != 0;
}

s21_decimal set_bit(s21_decimal value, int index) {
  value.bits[index / INT_SIZE] |= 1 << index % INT_SIZE;
  return value;
}

int get_significants(s21_decimal value) {
  int result = -1;
  for (int i = ALL_BITS - 1; i >= 0 && result == -1; i--) {
    if (get_bit(value, i)) result = i;
  }
  return result;
}

s21_decimal bin_add(s21_decimal d1, s21_decimal d2) {
  s21_decimal res = d1;
  s21_decimal tmp = d2;
  while (!is_null(tmp)) {
    s21_decimal of = decimal_logic(res, tmp, _and);
    of = bshift(of, LEFT, 1);
    res = decimal_logic(res, tmp, _xor);
    tmp = of;
  }
  return res;
}

big_decimal bd_bin_add(big_decimal d1, big_decimal d2) {
  big_decimal res = d1;
  big_decimal tmp = d2;
  while (!is_null(tmp.decimals[0]) || !is_null(tmp.decimals[1])) {
    big_decimal of;
    of.decimals[0] = decimal_logic(res.decimals[0], tmp.decimals[0], _and);
    of.decimals[1] = decimal_logic(res.decimals[1], tmp.decimals[1], _and);
    of = bdshift(of, LEFT, 1);
    res.decimals[0] = decimal_logic(res.decimals[0], tmp.decimals[0], _xor);
    res.decimals[1] = decimal_logic(res.decimals[1], tmp.decimals[1], _xor);
    tmp = of;
  }
  return res;
}

big_decimal bd_bin_sub(big_decimal d1, big_decimal d2) {
  big_decimal result;
  d2.decimals[0] = invert(d2.decimals[0]);
  d2.decimals[1] = invert(d2.decimals[1]);
  big_decimal one = tobd(Decimal(1));
  d2 = bd_bin_add(d2, one);
  result = bd_bin_add(d1, d2);
  return result;
}

int is_null(s21_decimal value) {
  return value.bits[0] == 0 && value.bits[1] == 0 && value.bits[2] == 0 &&
         value.bits[3] == 0;
}

int bd_is_null(big_decimal bd) {
  return is_null(bd.decimals[0]) && is_null(bd.decimals[1]);
}

int bin_cmp(s21_decimal d1, s21_decimal d2) {
  int result = 0;
  for (int i = ALL_BITS - 1; i >= 0 && result == 0; i--) {
    result = get_bit(d1, i) - get_bit(d2, i);
  }
  return result;
}

int bd_bin_cmp(big_decimal d1, big_decimal d2) {
  int compare = bin_cmp(d1.decimals[1], d2.decimals[1]);
  if (compare == 0) {
    compare = bin_cmp(d1.decimals[0], d2.decimals[0]);
  }
  return compare;
}

big_decimal bd_bin_mul(big_decimal d1, s21_decimal d2) {
  big_decimal res = tobd(Decimal(0));
  big_decimal tmp = d1;
  int significants = get_significants(d2);
  for (int i = 0; i <= significants; i++) {
    if (get_bit(d2, i) != 0) {
      res = bd_bin_add(res, tmp);
    }
    tmp = bdshift(tmp, LEFT, 1);
  }
  return res;
}

big_decimal bd_bin_div(big_decimal d1, big_decimal d2, big_decimal *rem) {
  big_decimal remainder = tobd(Decimal(0));
  big_decimal quotient = tobd(Decimal(0));
  if (bd_bin_cmp(d1, d2) == -1) {
    remainder = d1;
  } else if (!bd_is_null(d1)) {
    int signs1_1 = get_significants(d1.decimals[1]);
    int signs1_0 = get_significants(d1.decimals[0]);
    signs1_1 = signs1_1 == -1 ? signs1_0 : ALL_BITS + signs1_1;
    int signs2_1 = get_significants(d2.decimals[1]);
    int signs2_0 = get_significants(d2.decimals[0]);
    signs2_1 = signs2_1 == -1 ? signs2_0 : ALL_BITS + signs2_1;
    int shift = signs1_1 - signs2_1;
    big_decimal divisor = bdshift(d2, LEFT, shift);
    big_decimal dividend = d1;
    for (; shift >= 0; shift--) {
      quotient = bdshift(quotient, LEFT, 1);
      if (bd_bin_cmp(dividend, divisor) != -1) {
        dividend = bd_bin_sub(dividend, divisor);
        quotient.decimals[0] = set_bit(quotient.decimals[0], 0);
      }
      divisor = bdshift(divisor, RIGHT, 1);
    }
    remainder = dividend;
  }
  if (rem != NULL) *rem = remainder;
  return quotient;
}

int _and(int a, int b) { return a & b; }
int _xor(int a, int b) { return a ^ b; }

s21_decimal decimal_logic(s21_decimal d1, s21_decimal d2, Op op) {
  s21_decimal result = Decimal(0);
  for (int i = 0; i < 4; i++) {
    result.bits[i] = op(d1.bits[i], d2.bits[i]);
  }
  return result;
}

s21_decimal invert(s21_decimal value) {
  s21_decimal result = Decimal(0);
  for (int i = 0; i < 4; i++) {
    result.bits[i] = ~value.bits[i];
  }
  return result;
}

s21_decimal bshift(s21_decimal value, int dir, int amount) {
  s21_decimal result = value;
  for (int count = 0; count < amount; count++) {
    int carry = 0;
    for (int i = 0; i < 4; i++) {
      int j = dir ? i + 1 : 3 - i;
      int temp = get_bit(result, INT_SIZE * j - dir);
      if (dir) {
        result.bits[i] <<= 1;
        if (carry) result = set_bit(result, INT_SIZE * i);
      } else {
        unsigned int bits = result.bits[j];
        result.bits[j] = bits >> 1;
        if (carry) result = set_bit(result, INT_SIZE * (j + 1) - 1);
      }
      carry = temp;
    }
  }
  return result;
}

big_decimal bdshift(big_decimal value, int dir, int amount) {
  big_decimal result = value;
  for (int count = 0; count < amount; count++) {
    int j = dir ? ALL_BITS - 1 : 0;
    int bit = get_bit(result.decimals[1 - dir], j);
    result.decimals[0] = bshift(result.decimals[0], dir, 1);
    result.decimals[1] = bshift(result.decimals[1], dir, 1);
    if (bit) {
      result.decimals[dir] =
          set_bit(result.decimals[dir], dir ? 0 : ALL_BITS - 1);
    }
  }
  return result;
}
