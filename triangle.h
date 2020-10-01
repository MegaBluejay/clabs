#ifndef TRIANGLE_H
#define TRIANGLE_H

#include <math.h>

typedef struct {
  double x,y;
} vector;
typedef vector point;
typedef point triangle[3];

extern vector vec(point from, point to);
extern double norm(vector v);
extern double dot(vector v1, vector v2);
extern double cross(vector v1, vector v2);
extern double distance(point p1, point p2);
extern double area(triangle tri);
extern double perimeter(triangle tri);
extern int is_right(triangle tri);

#endif
