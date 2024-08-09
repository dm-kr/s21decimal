#ifndef SRC_UTILITY_H_
#define SRC_UTILITY_H_

#include "s21_decimal.h"

int get_decimal_shift(big_decimal value);
int s21_max(int value_1, int value_2);
s21_decimal decimal_abs(s21_decimal value_1);
int calc_fractional(big_decimal *quotient, big_decimal divisor,
                    big_decimal *rem);
void rescaling(s21_decimal d1, s21_decimal d2, big_decimal *bd1,
               big_decimal *bd2);
int get_exp(char *str);
s21_decimal float_string_to_decimal(char *str);

void clear(s21_decimal *decimal);
void clear_service(s21_decimal *value);
s21_decimal Decimal(int number);
s21_decimal ten_pow(int pow);
s21_decimal zerofive(void);
s21_decimal max_decimal(void);
s21_decimal max_int(void);
s21_decimal min_int(void);
big_decimal tobd(s21_decimal value);

int is_correct(s21_decimal value);
int get_sign(s21_decimal value);
int get_scale(s21_decimal value);
int get_empty1(s21_decimal value);
int get_empty2(s21_decimal value);
void set_sign(s21_decimal *value, int sign);
void set_scale(s21_decimal *decimal, int scale);
int is_even(s21_decimal value);

s21_decimal remove_trailing_zeros(s21_decimal value);
s21_decimal round_banking(s21_decimal integral, s21_decimal fractional);

#endif