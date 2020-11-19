#include <stdio.h>

const int MAX_LINE_LEN = 300;

int main(int argc, char *argv[]) {
  char line[MAX_LINE_LEN];
  FILE *file_to_concat_to = fopen(argv[3], "w");
  
  for (int i=1; i<=2; i++) {
    FILE *file_to_concat = fopen(argv[i], "r");
    while (fgets(line, sizeof(line), file_to_concat)) {
      fprintf(file_to_concat_to, "%s", line);
    }
    fclose(file_to_concat);
  }

  fclose(file_to_concat_to);
}
