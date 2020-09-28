#include <stdio.h>
#include <regex.h>
#include <string.h>

int isInt(char str[], int n) {
  const char digits[] = "0123456789";
  if (str[0]=='0' && n==1) {
    return 1;
  }
  int ans = 1;
  for (int i=0;i<n;++i) {
    int isDigit = 0;
    for (int j=(i==0);j<10;++j) {
      isDigit |= str[i]==digits[j];
    }
    ans &= isDigit;
  }
  return ans;
}

int main() {
  regex_t regex;
  char str[100];
  int regmatch, mymatch;

  regcomp(&regex, "[1-9][0-9]*|0", REG_EXTENDED);

  fgets(str, sizeof(str), stdin);
  str[strlen(str)-1] = 0;

  regmatch = !regexec(&regex, str, 0, NULL, 0);
  mymatch = isInt(str,strlen(str));
  
  if (mymatch) {
    int i;
    sscanf(str,"%d",&i);
    printf("int: %d\n",i);
  }
  else {
    printf("string: %s\n",str);
  }
}
