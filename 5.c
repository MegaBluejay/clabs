#include <stdio.h>

int main() {
  int arr[] = {77,12,74,34,56,78,234,678};

  for (int i=0; i<sizeof(arr)/sizeof(arr[0]); i++) {
    printf("%d ", arr[i]);
  }
  printf("\n");

  int m1[2][2] = {{1,0},{1,4}};
  int m2[2][2] = {{1,2},{0,1}};

  int mres[2][2];
  for (int i=0; i<sizeof(m1)/sizeof(m1[0]); i++) {
    for (int j=0; j<sizeof(m2[0])/sizeof(m2[0][0]); j++) {
      mres[i][j] = 0;
      for (int k=0; k<sizeof(m1[0])/sizeof(m1[0][0]); k++) {
        mres[i][j] += m1[i][k]*m2[k][j];
      }
    }
  }

  for (int i=0; i<sizeof(mres)/sizeof(mres[0]); i++) {
    for (int j=0; j<sizeof(mres[0])/sizeof(mres[0][0]); j++) {
      printf("%d ", mres[i][j]);
    }
    printf("\n");
  }
}
