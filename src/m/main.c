#include "../binary.h"
#include "../handlers.h"
#include "../s21_decimal.h"
#include "../utility.h"

int main() {
  s21_decimal a = Decimal(11);
  s21_decimal b = Decimal(3);
  big_decimal c = bd_bin_div(tobd(a), tobd(b), NULL);
  for (int i = BASE_SIZE; i >= 0; i--) {
    printf("%d", get_bit(c.decimals[0], i));
  }
  return 0;
}