#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>

// 3
int *digits(int n) {
  int len = floor(log10(n))+1;
  int *res = malloc(len*sizeof(int));
  for (int i=0; i<len; i++) {
    res[len-1-i] = n%10;
    n /= 10;
  }
  return res;
}

// 5

int isExtra(char* str, char *p, int openSingle, int openDouble) {
  if (*p != ' ') {
    return 0;
  }
  if ((*(p+1) == ' ') ||
      (*(p+1) == '\0' && (p==str || *(p-1) == '.' || *(p-1) == ' ')) ||
      (*(p-1) == '\'' && openSingle) ||
      (*(p-1) == '\"' && openDouble) ||
      (*(p-1) == '(') ||
      (*(p-1) == '[') ||
      (*(p-1) == '{')) {
    return 1;
  }
  return 0;
}

void fix(char *s, char *p, int os, int od) {
  if (s==p) {
    return;
  }
  if (isExtra(s,p,os,od)) {
    char *t = p;
    while (*t != '\0') {
      *t = *(t+1);
      t++;
    }
  }
  else if (*p == '\'') {
    os = !os;
  }
  else if (*p == '\"') {
    od = !od;
  }
  fix(s,p-1,os,od);
}

int main() {
  // 3
  int n;
  scanf("%d", &n);
  int *ds = digits(n);
  for (int i=0; i<floor(log10(n))+1; i++) {
    printf("%d ", ds[i]);
  }
  printf("\n");

  // 5
  char *str;
  fgets(str, 100, stdin);
  fix(str, strchr(str,'\0'), 0, 0);
  printf("%s\n", str);
  
  return 0;
}
