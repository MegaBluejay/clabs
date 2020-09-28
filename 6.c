#include <stdio.h>
#include <stdlib.h>

int main() {
  int arr[4];
  for (int i=0;i<4;++i) {
    arr[i] = 1000 + i;
  }
  for (int i=0;i<4;++i) {
    printf("%d ", *(arr+i));
  }
  printf("\n");

  int *darr = malloc(sizeof(int)*4);
  for (int i=0;i<4;++i) {
    darr[i] = 1000 + i;
  }
  for (int i=0;i<4;++i) {
    printf("%d ", darr[i]);
  }
  printf("\n");

  free(darr);
}
