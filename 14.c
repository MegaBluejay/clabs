#include <stdio.h>
#include <string.h>
#include <stdlib.h>

typedef struct {
  int *array;
  int has;
  int n;
} dynamic;

dynamic *init_dyn (int n) {
  dynamic *dyn = malloc(sizeof(dynamic));
  dyn->n = n;
  dyn->has = 0;
  dyn->array = malloc(sizeof(int)*n);
}

void del_dyn (dynamic *dyn) {
  free(dyn->array);
  free(dyn);
}

void print_cells(dynamic *dyn) {
  for (int i=0; i<dyn->has/2; i++) {
    printf("(%d, %d), ", dyn->array[i*2], dyn->array[i*2+1]);
  }
  printf("\n");
}

void append_cell(dynamic *cells, int x, int y) {
  if (cells->has+2>cells->n) {
    cells->n *= 2;
    int *new_array = malloc(sizeof(int)*cells->n);
    memcpy(new_array, cells->array, cells->has);
    free(cells->array);
    cells->array = new_array;
  }
  cells->array[cells->has] = x;
  cells->array[cells->has+1] = y;
  cells->has += 2;
}

int has_cell(dynamic *cells, int x, int y) {
  for (int i=0; i<cells->has/2; i++) {
    if (cells->array[i*2]==x && cells->array[i*2+1]==y) {
      return 1;
    }
  }
  return 0;
}

int pop_cell (dynamic *cells, int x, int y) {
  for (int i=0; i<cells->has/2; i++) {
    if (cells->array[i*2]==x && cells->array[i*2+1]==y) {
      cells->array[i*2] = -1;
      cells->array[i*2+1] = -1;
      return 1;
    }
  }
  return 0;
}

dynamic *next_gen(dynamic *live) {
  dynamic *new_live = init_dyn(live->n);
  dynamic *neigh1 = init_dyn(live->n*9);
  dynamic *neigh2 = init_dyn(live->n*9);
  dynamic *neigh3 = init_dyn(live->n*9);
  for (int i=0; i<live->has/2; i++) {
    int x = live->array[i*2];
    int y = live->array[i*2+1];
    int n_neighs = 0;
    for (int dx=-1; dx<=1; dx++) {
      for (int dy=-1; dy<=1; dy++) {
        if ((dx!=0 || dy!=0) && x+dx>=0 && y+dy>=0) {
          if (has_cell(live, x+dx, y+dy)) {
            n_neighs++;
          }
          else {
            if (pop_cell(neigh3, x+dx, y+dy)) {
              // over 3, don't care
            }
            else if (pop_cell(neigh2, x+dx, y+dy)) {
              append_cell(neigh3, x+dx, y+dy);
            }
            else if (pop_cell(neigh1, x+dx, y+dy)) {
              append_cell(neigh2, x+dx, y+dy);
            }
            else {
              append_cell(neigh1, x+dx, y+dy);
            }
          }
        }
      }
    }
    if (2<=n_neighs && n_neighs<=3) {
      append_cell(new_live, x, y);
    }
  }
  for (int i=0; i<neigh3->has/2; i++) {
    int x = neigh3->array[i*2];
    int y = neigh3->array[i*2+1];
    if (x!=-1 && y!=-1) {
      append_cell(new_live, x, y);
    }
  }
  del_dyn(neigh1);
  del_dyn(neigh2);
  del_dyn(neigh3);
  del_dyn(live);
  return new_live;
}

typedef struct {
  int *array;
  int h;
  int wi;
} bitmap;

void del_map(bitmap *map) {
  free(map->array);
  free(map);
}

bitmap *to_map(dynamic *live) {
  int mx = -1;
  for (int i=0; i<live->has/2; i++) {
    int x = live->array[i*2];
    int y = live->array[i*2+1];
    if (x>mx) {
      mx = x;
    }
    if (y>mx) {
      mx = y;
    }
  }
  int w = mx+1;
  int wi = ((w-1)>>5)+1;
  int h = wi<<5;
  bitmap *map = malloc(sizeof(bitmap));
  map->h = h;
  map->wi = wi;
  map->array = calloc(h*wi, sizeof(int));
  for (int i=0; i<live->has/2; i++) {
    int x = live->array[i*2];
    int y = live->array[i*2+1];
    int xi = x>>5;
    int xr = x-(xi<<5);
    map->array[y*wi+xi] |= 1<<(31-xr);
  }
  return map;
}

dynamic *from_map(bitmap *map) {
  dynamic *live = init_dyn(map->h*map->wi*32);
  for (int i=0; i<map->h; i++) {
    for (int j=0; j<map->wi; j++) {
      for (int k=0; k<32; k++) {
        int pixel = (map->array[i*map->wi+j]>>(31-k))&1;
        if (pixel) {
          append_cell(live, j*32+k, i);
        }
      }
    }
  }
  return live;
}

void write_bmp(bitmap *map, FILE* file) {
  fseek(file, 0, SEEK_SET);
  fputs("BM", file);
  int data_size = 4*map->h*map->wi;
  int offset = 62;
  int file_size = data_size+offset;
  fwrite(&file_size, 4, 1, file);
  for (int i=0; i<4; i++) {
    fputc(0, file);
  }
  fwrite(&offset, 4, 1, file);

  int header_size = 40;
  int w = map->wi*32;
  int one = 1;
  int zero = 0;
  int two = 2;
  fwrite(&header_size, 4, 1, file);
  fwrite(&w, 4, 1, file);
  fwrite(&map->h, 4, 1, file);
  fwrite(&one, 2, 1, file);
  fwrite(&one, 2, 1, file);
  for (int i=0; i<16; i++) {
    fputc(0, file);
  }
  fwrite(&two, 4, 1, file);
  for (int i=0; i<4; i++) {
    fputc(0, file);
  }

  for (int i=0; i<4; i++) {
    fputc(0, file);
  }
  for (int i=0; i<3; i++) {
    fputc(0xFF, file);
  }
  fputc(0, file);

  //  fwrite(map->array, 4, map->h*map->wi, file);
  for (int i=0; i<map->h*map->wi; i++) {
    char buf[4];
    char *t = (char *) &map->array[i];
    for (int j=0; j<4; j++) {
      buf[3-j] = *(t++);
    }
    fwrite(buf, 4, 1, file);
  }
}

bitmap *read_bmp(FILE* file) {
  fseek(file, 2, SEEK_SET); // skip signature
  int file_size;
  fread(&file_size, 4, 1, file);
  fseek(file, 4, SEEK_CUR); // skip reserved
  int data_offset;
  fread(&data_offset, 4, 1, file);
  
  fseek(file, 4, SEEK_CUR); // skip header size
  int w;
  fread(&w, 4, 1, file);
  int h;
  fread(&h, 4, 1, file);
  fseek(file, data_offset, SEEK_SET); // skip rest of header and color table
  bitmap *map = malloc(sizeof(bitmap));
  map->h = h;
  map->wi = ((w-1)>>5)+1;
  map->array = malloc(sizeof(int)*map->h*map->wi);
  //  fread(map->array, file_size-data_offset, 1, file);
  for (int i=0; i<map->h*map->wi; i++) {
    char buf[4];
    for (int j=0; j<4; j++) {
      fread(&buf[3-j], 1, 1, file);
    }
    memcpy(&map->array[i], buf, 4);
  }
  return map;
}

int main(int argc, char *argv[]) {
  char *input_str;
  char *output_str;
  int max_iter = -1;
  int dump_freq = 1;
  for (int i=1; i<argc; i+=2) {
    if (!strcmp(argv[i], "--input")) {
      input_str = argv[i+1];
    }
    else if (!strcmp(argv[i], "--output")) {
      output_str = argv[i+1];
    }
    else if (!strcmp(argv[i], "--max_iter")) {
      sscanf(argv[i+1], "%d", &max_iter);
    }
    else if (!strcmp(argv[i], "--dump_freq")) {
      sscanf(argv[i+1], "%d", &dump_freq);
    }
    else {
      printf("invalid args\n");
      exit(1);
    }
  }
  if (!input_str || !output_str) {
    printf("required args not given\n");
    exit(1);
  }

  dynamic *live;
  bitmap *map;
  FILE *init_file = fopen(input_str, "rb");
  map = read_bmp(init_file);
  fclose(init_file);
  live = from_map(map);
  del_map(map);
  
  char buffer[200];
  int q = strlen(output_str)+1;
  strcpy(buffer, output_str);
  buffer[q-1] = '/';
  for (int i=0; max_iter==-1 || i<max_iter; i++) {
    if (i%dump_freq == 0) {
      int w = q+sprintf(&buffer[q], "%d", i);
      sprintf(&buffer[w], "%s", ".bmp");
      
      map = to_map(live);
      FILE *file = fopen(buffer, "wb");
      write_bmp(map, file);
      fclose(file);
      del_map(map);
    }
    live = next_gen(live);
  }
}
