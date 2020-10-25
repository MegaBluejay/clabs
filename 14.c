#include <stdio.h>
#include <string.h>

int main(int argc, char *argv[]) {
  char *input_file, *output_dir;
  int max_iter, dump_freq=1;
  for (int i=1; i<argc; i++) {
    if (!strcmp(argv[i], "input")) {
      i++;
      input_file = argv[i];
    }
    else if (!strcmp(argv[i], "output")) {
      i++;
      output_dir = argv[i];
    }
    else if (!strcmp(argv[i], "max_iter")) {
      i++;
      sscanf(argv[i], "%d", &max_iter);
    }
    else if (!strcmp(argv[i], "dump_freq")) {
      i++;
      sscanf(argv[i], "%d", &dump_freq);
    }
  }
}
