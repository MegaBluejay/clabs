#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int mlen(char *s) {
  int n = 0;
  for (; s[n]!='\0'; n++) {}
  return n;
}

// 1
char *mcon(char *s1, char *s2) {
  int n1 = mlen(s1), n2 = mlen(s2);
  char *res = malloc((n1+n2+1)*sizeof(char));
  for (int i=0;i<n1;i++) {
    res[i] = s1[i];
  }
  for (int i=0;i<n2;i++) {
    res[i+n1] = s2[i];
  }
  res[n1+n2] = '\0';
  return res;
}

// 4
int mcmpn(char *s1, char *s2, int n) {
  for (int i=0;i<n;i++) {
    if (s1[i]<s2[i]) {
      return -1;
    }
    else if (s1[i]>s2[i]) {
      return 1;
    }
  }
  return 0;
}

// 5
void mcopy(char *dest, char *src) {
  for (int i=0;i<mlen(src)+1;i++) {
    dest[i] = src[i];
  }
}

// 8
int mfindfst(char *s, char c) {
  for (int i=0;i<mlen(s);i++) {
    if (s[i]==c) {
      return i;
    }
  }
  return -1;
}

// 11
int mlenin(char *s, char *set) {
  int n = 0;
  for (; mfindfst(set,s[n])!=-1; n++) {}
  return n;
}

int main() {
  // 1
  char s1[100] = "Hello ", s2[100] = "world!";
  printf("%s\n", mcon(s1,s2));

  // 4
  mcopy(s1,"manchester");
  mcopy(s2,"manliness");
  printf("%d\n", mcmpn(s1,s2,8));

  // 5
  mcopy(s1,s2);
  printf("%s\n", s1);

  // 8
  printf("%d\n", mfindfst(s1, 'n'));

  // 11
  printf("%d\n", mlenin(s1, "animal"));
  
  return 0;
}
