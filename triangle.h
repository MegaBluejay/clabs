#ifndef TRIANGLE_H
#define TRIANGLE_H

#include <math.h>

typedef struct {
  double x,y;
} vector;
typedef vector point;
typedef point triangle[3];

vector vec(point from, point to);
double norm(vector v);
double dot(vector v1, vector v2);
double cross(vector v1, vector v2);
double distance(point p1, point p2);
double area(triangle tri);
double perimeter(triangle tri);
int is_right(triangle tri);

#endif
