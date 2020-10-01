#include <stdio.h>
#include <time.h>

const int max_line_len = 300;

void task1(char *outfile) {
  FILE *dates_file = fopen(outfile,"w");
  const time_t day = (time_t) 60*60*24;
  time_t time_counter = time(NULL);
  char time_string[max_line_len];
  for (int i=0; i<10; i++) {
    strftime(time_string, sizeof(time_string), "%F", localtime(&time_counter));
    fprintf(dates_file, "%s\n",time_string);
    time_counter += day;
  }
  fclose(dates_file);
}

void task2(char *infile) {
  FILE *file_for_even = fopen(infile, "r");
  char line[max_line_len];
  for (int i=0; fgets(line, sizeof(line), file_for_even); i++) {
    if (!(i%2)) {
      printf("%s", line);
    }
  }
  fclose(file_for_even);
}

void task3(char *infile1, char *infile2, char *outfile) {
  char line[max_line_len];
  FILE *file_to_concat_to = fopen(outfile, "w");
  
  FILE *file_to_concat = fopen(infile1, "r");
  while (fgets(line, sizeof(line), file_to_concat)) {
    fprintf(file_to_concat_to, "%s", line);
  }

  freopen(infile2, "r", file_to_concat);
  while (fgets(line, sizeof(line), file_to_concat)) {
    fprintf(file_to_concat_to, "%s", line);
  }
  
  fclose(file_to_concat);
  fclose(file_to_concat_to);
}

void task4(char *a_string, char *b_string, char *outfile) {
  int a,b;
  sscanf(a_string, "%d", &a);
  sscanf(b_string, "%d", &b);
  freopen(outfile, "w", stdout);
  printf("%d\n", a+b);
  #ifdef __linux__
  freopen("/dev/tty", "a", stdout);
  #endif
  #ifdef _WIN32
  freopen("CON", "a", stdout);
  #endif
}

void task5(char *info_file, int nfiles, char *files_to_create[]) {
  FILE *info = fopen(info_file, "w");
  for (int i=0; i<nfiles; i++) {
    FILE *empty_file = fopen(files_to_create[i], "w");
    fclose(empty_file);
    fprintf(info, "created %s\n", files_to_create[i]);
  }
  fclose(info);
}

int main(int argc, char *argv[]) {
  int task_num;
  sscanf(argv[1], "%d", &task_num);
  if (task_num==1) {
    task1(argv[2]);
  }
  else if (task_num==2) {
    task2(argv[2]);
  }
  else if (task_num==3) {
    task3(argv[2], argv[3], argv[4]);
  }
  else if (task_num==4) {
    task4(argv[2], argv[3], argv[4]);
  }
  else if(task_num==5) {
    task5(argv[2], argc-3, &argv[3]);
  }
}
