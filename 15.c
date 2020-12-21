#include <stdio.h>
#include <string.h>
#include <stdlib.h>

const int MAX_FILENAME = 300;

void pack(int n_files, char *filenames[], char *out_filename) {
  FILE *out_file = fopen(out_filename, "wb");
  fputs("ARC", out_file);
  for (int i=0; i<n_files; i++) {
    FILE *in_file = fopen(filenames[i], "rb");
    fseek(in_file, 0, SEEK_END);
    int file_size = ftell(in_file);
    int name_size = strlen(filenames[i]);

    fwrite(&name_size, 4, 1, out_file);
    fwrite(&file_size, 4, 1, out_file);
    fputs(filenames[i], out_file);

    char *data = malloc(file_size);
    fseek(in_file, 0, SEEK_SET);
    fread(data, 1, file_size, in_file);
    fclose(in_file);
    fwrite(data, 1, file_size, out_file);
    free(data);
  }
  fclose(out_file);
}

void unpack(char *arc_filename) {
  FILE *arc_file = fopen(arc_filename, "rb");
  char buf[MAX_FILENAME];
  fread(buf, 1, 3, arc_file);
  if (strncmp(buf, "ARC", 3)) {
    printf("invalid archive\n");
    return;
  }
  for (;;) {
    int name_size;
    int file_size;
    if (!fread(&name_size, 4, 1, arc_file)) {
      break;
    }
    fread(&file_size, 4, 1, arc_file);
    fgets(buf, name_size+1, arc_file);
    printf("%d %d %s\n", name_size, file_size, buf);
    char *data = malloc(file_size);
    fread(data, 1, file_size, arc_file);
    FILE *unarched_file = fopen(buf, "wb");
    fwrite(data, 1, file_size, unarched_file);
    fclose(unarched_file);
    free(data);
  }
  fclose(arc_file);
}

void print_list(char *arc_filename) {
  FILE *arc_file = fopen(arc_filename, "rb");
  char buf[MAX_FILENAME];
  fread(buf, 1, 3, arc_file);
  if (strncmp(buf, "ARC", 3)) {
    printf("invalid archive\n");
    return;
  }
  for (int i=0;; i++) {
    int name_size;
    int file_size;
    if (!fread(&name_size, 4, 1, arc_file)) {
      break;
    }
    fread(&file_size, 4, 1, arc_file);
    fgets(buf, name_size+1, arc_file);
    printf("%s\n", buf);
    fseek(arc_file, file_size, SEEK_CUR);
  }
  fclose(arc_file);
}

int main(int argc, char *argv[]) {
  char *arc_filename;
  int create = 0;
  int extract = 0;
  int list = 0;
  int i = 0;
  for (i=1; i<argc && !strncmp(argv[i], "--", 2); i++) {
    if (!strcmp(argv[i], "--file")) {
      arc_filename = argv[++i];
    }
    else if (!strcmp(argv[i], "--create")) {
      create = 1;
    }
    else if (!strcmp(argv[i], "--extract")) {
      extract = 1;
    }
    else if (!strcmp(argv[i], "--list")) {
      list = 1;
    }
    else {
      printf("invalid args\n");
      exit(1);
    }
  }
  if (!arc_filename || (create + extract + list != 1)) {
    printf("necessary args not given\n");
    exit(1);
  }
  if (create) {
    pack(argc-i, &argv[i], arc_filename);
  }
  else if (extract) {
    unpack(arc_filename);
  }
  else if (list) {
    print_list(arc_filename);
  }
}
