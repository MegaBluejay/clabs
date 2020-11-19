#include <stdio.h>
#include <math.h>

// 3
const char* digits[] = {"zero", "one", "two", "three", "four", "five", "six", "seven", "eight", "nine"};

int main() {
  // 3
  int digit;
  scanf("%d", &digit);
  printf("%s\n", digits[digit]);

  // 5
  int n_months;
  double yearly_perc, balance;
  scanf("%d", &n_months);
  scanf("%lf", &yearly_perc);
  scanf("%lf", &balance);
  double monthly = pow(yearly_perc/100+1, (double) 1/12);
  for (int i=0; i<=n_months; i++) {
    printf("%f ", balance);
    balance *= monthly;
  }
  printf("\n");

  return 0;
}
