#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int ceil_div(int x, int y) {
  return (x+y-1)/y;
}

typedef struct {
  int* words;
  int w;
  int h;
  int n_alive;
} bitmap;

int get_bit(bitmap* bits, int x, int y) {
  int i = y*(bits->w)+x;
  return bits->words[i/(sizeof(int)*8)] & (1<<(i%(sizeof(int)*8)));
}
void set_bit(bitmap* bits, int x, int y) {
  int i = y*(bits->w)+x;
  bits->words[i/(sizeof(int)*8)] |= (1<<(i%(sizeof(int)*8)));
}

void bits_init(bitmap* bits, int w, int h) {
  bits->w = w;
  bits->h = h;
  bits->n_alive = 0;
  int n_words = ceil_div(w*h, 8*sizeof(int));
  bits->words = calloc(sizeof(int), n_words);
}
void bits_free(bitmap* bits) {
  free(bits->words);
}

typedef struct {
  int* array;
  size_t used;
  size_t size;
} dynamic;

void dyn_init(dynamic* dyn, size_t size) {
  dyn->array = malloc(size*sizeof(int));
  dyn->used = 0;
  dyn->size = size;
}

void dyn_append(dynamic* dyn, int x) {
  if (dyn->used == dyn->size) {
    dyn->size *= 2;
    dyn->array = realloc(dyn->array, dyn->size * sizeof(int));
  }
  dyn->array[dyn->used] = x;
  dyn->used++;
}

void dyn_free(dynamic* dyn) {
  free(dyn->array);
}

typedef struct {
  dynamic* alive;
  int n;
} board;

void brd_init(board* brd, int n) {
  brd->n = n;
  dyn_init(brd->alive, n*2);
}

void brd_free(board* brd) {
  dyn_free(brd->alive);
}

void from_bitmap(board* brd, bitmap* bits, int n_alive) {
  brd_init(brd, n_alive);
  for(int j=0; j < bits->h; j++) {
    for (int i=0; i < bits->w; i++) {
      if (get_bit(bits, i, j)) {
        dyn_append(brd->alive, i);
        dyn_append(brd->alive, j);
      }
    }
  }
}

void to_bitmap(bitmap* bits, board* brd) {
  int w=0, h=0;
  for (int i=0; i<brd->n, i++) {
    int x = brd->alive->array[i*2];
    int y = brd->alive->array[i*2+1];
    if (x+1>w) {
      w = x+1;
    }
    if (y+1>h) {
      h = y+1;
    }
  }
  bits_init(bits, w, h);
  bits->n_alive = brd->n;
  for (int i=0; i<brd->n; i++) {
    int x = brd->alive->array[i*2];
    int y = brd->alive->array[i*2+1];
    set_bit(bits, x, y);
  }
} 

int is_alive(board* brd, int x, int y) {
  for (int i=0; i<brd->n; i++) {
    if(brd->alive->array[i*2]==x && brd->alive->array[i*2+1]==y) {
      return 1;
    }
  }
  return 0;
}

void next_gen(board* new_brd, board* brd) {
  brd_init(new_brd, brd->n);
  for (int k=0; k<brd->n; k++) {
    int x = brd->alive->array[k*2];
    int y = brd->alive->array[k*2+1];
    int neighs_alive_main = 0;
    for (int i=-1; i<=1; i++) {
      for (int j=-1; j<=1; j++) {
        if ((i!=0 || j!=0) && x+i>=0 && y+j>=0) {
          if (is_alive(brd, x+i, y+j)) {
            neighs_alive_main++;
          }
          else {
            int neighs_alive_second = 0;
            for (int i2=-1; i2<=1; i2++) {
              for (int j2=-1; j2<=1; j2++) {
                if (is_alive(brd, x+i+i2, y+j+j2)) {
                  neighs_alive_second++;
                }
              }
            }
            if(neighs_alive_second==3) {
              dyn_append(new_brd->alive, x+i);
              dyn_append(new_brd->alive, y+j);
            }
          }
        }
      }
    }
    if (2 <= neighs_alive_main && neighs_alive_main <= 3) {
      dyn_append(new_brd->alive, x);
      dyn_append(new_brd->alive, y);
    }
  }
  brd_free(brd);
}

int write_zeros(int n, FILE* file) {
  for (int i=0; i<n; i++) {
    int c = fputc(0, file);
    if (c==EOF) {
      return i;
    }
  }
  return n;
}

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
