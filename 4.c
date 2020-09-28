#include <stdio.h>

int main() {
  int x;
  scanf("%d", &x);

  printf("%d\n", (-77 <= x) && (x <= 1));

  int y;
  scanf("%d", &y);

  printf("%d\n", (y & (1 << (18-1))) != 0);
  /* printf("%d\n", (y & (1 << (sizeof(int)*8-18)) != 0); */
}
