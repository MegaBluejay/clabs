#include <stdio.h>
#include <string.h>

int main(int argc, char *argv[]) {
  int show=0, set=0, get=0;
  char *filepath, *prop_name, *prop_value;
  for (int i=1; i<argc; i++) {
    char *arg_name;
    arg_name = strtok(&(argv[i][2]), "=");
    if (!strcmp(arg_name, "filepath")) {
      filepath = strtok(NULL, "=");
    }
    else if (!strcmp(arg_name, "show")) {
      show = 1;
    }
    else if (!strcmp(arg_name, "set")) {
      set = 1;
      prop_name = strtok(NULL, "=");
    }
    else if (!strcmp(arg_name, "value")) {
      prop_value = strtok(NULL, "=");
    }
    else if (!strcmp(arg_name, "get")) {
      get = 1;
      prop_name = strtok(NULL, "=");
    } 
  }
  printf("%s\n", filepath);
  if (show) {
    printf("show\n");
  }
  if (set || get) {
    if (set) {
      printf("set\n");
    }
    if (get) {
      printf("get\n");
    }
    printf("%s\n", prop_name);
    if (set) {
      printf("%s\n", prop_value);
    }
  }
}
