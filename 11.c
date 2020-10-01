#include <stdio.h>
#include "triangle.h"

void read_triangle(triangle out) {
  for(int i=0;i<3;i++) {
    scanf("%lf %lf",&out[i].x,&out[i].y);
  }
}

int main() {
  triangle tri;
  read_triangle(tri);
  if (is_right(tri)) {
    printf("perimeter: %f\narea: %f\n", perimeter(tri), area(tri));
  }
  else {
    printf("not right");
  }
}
