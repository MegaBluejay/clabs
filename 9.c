#include <stdio.h>
#include <math.h>

// Something feels off here, too easy

// 3
const char *digits[] = {"zero", "one", "two", "three", "four", "five", "six", "seven", "eight", "nine"};

int main() {
  // 3
  int digit;
  scanf("%d", &digit);
  printf("%s\n", digits[digit]);

  // 5
  double yearly_perc, balance;
  scanf("%lf", &yearly_perc);
  scanf("%lf", &balance);
  double monthly = pow(yearly_perc/100+1, (double) 1/12);
  for (int i=0; i<=12; i++) {
    printf("%f ", balance);
    balance *= monthly;
  }
  printf("\n");

  return 0;
}
