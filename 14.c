#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
  int* array;
  int has;
  int size;
} dynamic;

void init(dynamic* dyn, int size) {
  dyn->size = size;
  dyn->has = 0;
  dyn->array = malloc(sizeof(int)*size);
}

void dyn_free(dynamic* dyn) {
  free(dyn->array);
  dyn->has = 0;
  dyn->size = 0;
}

void append_cell(dynamic* live, int x, int y) {
  live->has += 2;
  if (live->has >= live->size) {
    live->size = live->has * 2;
    live->array = realloc(live->array, live->size);
  }
  live->array[live->has-2] = x;
  live->array[live->has-1] = y;
}

int is_neigh(int x1, int y1, int x2, int y2) {
  return abs(x1-x2)<=1 && abs(y1-y2)<=1 && (x1!=x2 || y1!=y2);
}

dynamic* next_gen(dynamic* live) {
  dynamic* new_live;
  init(new_live, live->has * 2);
  for (int i=0; i < live->has / 2; i+=2) {
    int x = live->array[i];
    int y = live->array[i+1];
    int n_main = 0;
    for (int j=0; j < live->has / 2; j+=2) {
      int x_neigh = live->array[j];
      int y_neigh = live->array[j+1];
      if (is_neigh(x, y, x_neigh, y_neigh)) {
        n_main+=1;
      }
    }
    if (2<=n_main && n_main<=3) {
      append_cell(new_live, x, y);
    }
    for (int dx=-1; dx<=1; dx++) {
      for (int dy=-1; dy<=1; dy++) {
        int n_second = 0;
        for (int j=0; j < live->has / 2; j+=2) {
          int x_neigh = live->array[j];
          int y_neigh = live->array[j+1];
          if (x_neigh==x+dx && y_neigh==y+dy) {
            n_second = 0;
            break;
          }
          if (is_neigh(x+dx, y+dy, x_neigh, y_neigh)) {
            n_second+=1;
          }
        }
        if (n_second==3) {
          append_cell(new_live, x+dx, y+dy);
        }
      }
    }
  }
  dyn_free(live);
  return new_live;
}

int ceil_div(int x, int y) {
  return (x+y-1)/y;
}

void read_bmp(FILE* file, dynamic* live) {
  int data_offset;
  int w,h;
  fseek(file, 10, SEEK_CUR);
  fread(&data_offset, 4, 1, file);
  fseek(file, 4, SEEK_CUR);
  fread(&w, 4, 1, file);
  fread(&h, 4, 1, file);

  fseek(file, data_offset, SEEK_SET);
  int line_len = ceil_div(w, 32);
  init(live, w*h/5);
  int* line = malloc(4*line_len);
  for (int i=0; i<h; i++) {
    fread(&line, 4, line_len, file);
    for (int j=0; j<w; j++) {
      int pixel = (line[j/32]>>(31-j%32))&1;
      if (pixel) {
        append_cell(live, j, i);
      }
    }
  }
  free(line);
}

void write_bmp(FILE* file, dynamic* live) {
  fputs("BM", file);

  int w = 0, h = 0;
  for (int i=0; i < live->has / 2; i+=2) {
    int x = live->array[i];
    int y = live->array[i+1];
    if (x+1>w) {
      w = x+1;
    }
    if (y+1>h) {
      h = y+1;
    }
  }
  int line_len = 32*ceil_div(w, 32);
  int data_size = line_len*h;
  int offset = 14+40+8;
  int full_size = offset+data_size;

  fwrite(&full_size, 4, 1, file);
  for (int i=0; i<4; i++) {
    fputc(0, file);
  }
  fwrite(&offset, 4, 1, file);

  int header_size = 40;
  short planes = 1;
  short bpp = 1;
  int total_colors = 2;
  fwrite(&header_size, 4, 1, file);
  fwrite(&w, 4, 1, file);
  fwrite(&h, 4, 1, file);
  fwrite(&planes, 2, 1, file);
  fwrite(&bpp, 2, 1, file);
  for (int i=0; i<16; i++) {
    fputc(0, file);
  }
  fwrite(&total_colors, 4, 1, file);
  for (int i=0; i<4; i++) {
    fputc(0, file);
  }

  for (int i=0; i<4; i++) {
    fputc(0, file);
  }
  for (int i=0; i<3; i++) {
    fputc(255, file);
  }
  fputc(0, file);

  int* bitmap = calloc(data_size, 1);
  for (int i=0; i < live->has / 2; i+=2) {
    int x = live->array[i];
    int y = live->array[i+1];
    bitmap[y*line_len+x/32] |= 1<<(31-x%32);
  }
  fwrite(&bitmap, data_size, 1, file);
  free(bitmap);
}

/* int main(int argc, char *argv[]) { */
/*   char *input_file, *output_dir; */
/*   int max_iter, dump_freq=1; */
/*   for (int i=1; i<argc; i++) { */
/*     if (!strcmp(argv[i], "input")) { */
/*       input_file = argv[++i]; */
/*     } */
/*     else if (!strcmp(argv[i], "output")) { */
/*       output_dir = argv[++i]; */
/*     } */
/*     else if (!strcmp(argv[i], "max_iter")) { */
/*       sscanf(argv[++i], "%d", &max_iter); */
/*     } */
/*     else if (!strcmp(argv[i], "dump_freq")) { */
/*       sscanf(argv[++i], "%d", &dump_freq); */
/*     } */
/*   } */
/*   if (!input_file || !output_dir) { */
/*     printf("arguement error"); */
/*     exit(1); */
/*   } */
/*   FILE* ifile = fopen(input_file, "rb"); */
/*   dynamic* live; */
/*   read_bmp(ifile, live); */
/*   fclose(ifile); */
/*   char* ofile_path = malloc(strlen(output_dir)+19); */
/*   char* number = ofile_path+strlen(output_dir); */
/*   strcpy(ofile_path, output_dir); */
/*   strcpy(number, "/gol_"); */
/*   number += 5; */
/*   for (int i=1; max_iter==0 || i<=max_iter; i++) { */
/*     live = next_gen(live); */
/*     if (i%dump_freq == 0) { */
/*       int numlen = sprintf(number, "%d", i); */
/*       strcpy(number+numlen, ".bmp"); */
/*       FILE* ofile = fopen(ofile_path, "wb"); */
/*       write_bmp(ofile, live); */
/*       fclose(ofile); */
/*     } */
/*   } */
/*   free(ofile_path); */
/* } */

int main() {
  dynamic* live;
  init(live,10);
  append_cell(live, 5,5);
  append_cell(live, 5,6);
  append_cell(live, 5,4);
  FILE* ifile = fopen("./test.bmp", "wb");
  write_bmp(ifile, live);
  fclose(ifile);
  dyn_free(live);
}
