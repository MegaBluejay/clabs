#include <stdio.h>

// 1
enum lamp { FLUORESCENT, HALOGEN, ARC, LED, COLD_CATHODE };

// 2
typedef struct {
  double x, y, w, h;
} rect;

double area(rect r) {
  return r.w*r.h;
}

// 3
typedef union {
  struct {
    unsigned int play: 1;
    unsigned int pause: 1;
    unsigned int rec: 1;
  };
  unsigned int repr;
} mp3_status;

int main() {
  // 1
  printf("%d\n", HALOGEN);

  // 2
  rect r = {0, 0, 10, 20};
  printf("%f\n", area(r));

  // 3
  mp3_status status;
  scanf("%x", &status);
  printf("%d %d %d\n", status.play, status.pause, status.rec);
}
