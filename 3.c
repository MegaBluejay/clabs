#include <stdio.h>

int main() {
  int x;
  scanf("%x", &x);

  printf("%o\n", x);
  printf("%x %x\n", x, x<<2);
  printf("%x %x\n", x, ~x);
  
  int y;
  scanf("%x", &y);
  printf("%x\n", x | y);
}
