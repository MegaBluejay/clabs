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
void fix(char *string, char *current, int open_single_quote, int open_double_quote) {
  if (*current == ' ') {
    if (*(current+1) == '\0' ||
        current-1>=string && (*(current-1) == ' ' && *(current+1) == ' ' ||
                              *(current-1) == '(' ||
                              *(current-1) == '[' ||
                              *(current-1) == '{' ||
                              *(current-1) == '\'' && open_single_quote ||
                              *(current-1) == '\"' && open_double_quote)) {
      char *temp = current;
      while(*temp != '\0') {
        *temp = *(temp+1);
        temp++;
      }
      current++;
    }
  }
  else if (*current == '\'') {
    open_single_quote = !open_single_quote;
  }
  else if (*current == '\"') {
    open_double_quote = !open_double_quote;
  }
  if (current==string) {
    return;
  }
  fix(string, current-1, open_single_quote, open_double_quote);
}

int main() {
  // 3
  int n;
  scanf("%d\n", &n);
  int *ds = digits(n);
  for (int i=0; i<floor(log10(n))+1; i++) {
    printf("%d ", ds[i]);
  }
  printf("\n");

  // 5
  char string[100];
  fgets(string, 100, stdin);
  char *last = strchr(string,'\0')-1;
  *last = '\0';
  last-=1;
  fix(string, last, 0, 0);
  printf("%s\n", string);
  
  return 0;
}
