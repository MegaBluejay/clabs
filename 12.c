#include <stdio.h>
#include <time.h>

const int MAX_LINE_LEN = 300;

int main(int argc, char *argv[]) {
  char line[MAX_LINE_LEN];
  FILE *file_to_concat_to = fopen(argv[3], "w");
  
  FILE *file_to_concat = fopen(argv[1], "r");
  while (fgets(line, sizeof(line), file_to_concat)) {
    fprintf(file_to_concat_to, "%s", line);
  }

  freopen(argv[2], "r", file_to_concat);
  while (fgets(line, sizeof(line), file_to_concat)) {
    fprintf(file_to_concat_to, "%s", line);
  }
  
  fclose(file_to_concat);
  fclose(file_to_concat_to);
}
