#include <stdio.h>
#include <math.h>

double z1(double a) {
  return ((a + 2)/sqrt(2 * a) - a / (sqrt(2 * a) + 2) + 2 / (a - sqrt(2 * a))) * (sqrt(a) - sqrt(2)) / (a + 2);
}

double z2(double a) {
  return 1 / (sqrt(a) + sqrt(2));
}

int main(){
  double a1, a2;
  scanf("%lf", &a1);
  scanf("%lf", &a2);

  printf("%f\n%f\n", z1(a1), z2(a2));
}
