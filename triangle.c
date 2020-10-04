#include "triangle.h"

/* typedef struct { */
/*   double x,y; */
/* } vector; */
/* typedef vector point; */
/* typedef point triangle[3]; */

vector vec(point from, point to) {
  vector res;
  res.x = to.x-from.x;
  res.y = to.y-from.y;
  return res;
}

double norm(vector v) {
  return sqrt(pow(v.x, 2) + pow(v.y, 2));
}

double dot(vector v1, vector v2) {
  return v1.x*v2.x + v1.y*v2.y;
}

double cross(vector v1, vector v2) {
  return v1.x*v2.y - v1.y*v2.x;
}

double distance(point p1, point p2) {
  return norm(vec(p1, p2));
}

double area(triangle tri) {
  vector a = vec(tri[0], tri[1]);
  vector b = vec(tri[0], tri[2]);
  return fabs(cross(a, b))/2;
}

double perimeter(triangle tri) {
  double res = 0;
  for(int i=0;i<3;i++) {
    res += distance(tri[i],tri[(i+1)%3]);
  }
  return res;
}

int is_right(triangle tri) {
  int right = 0;
  for(int i=0; i<3; i++) {
    vector side1 = vec(tri[i], tri[(i+1)%3]);
    vector side2 = vec(tri[(i+1)%3], tri[(i+2)%3]);
    right |= fabs(dot(side1, side2)) < 0.0001;
  }
  return right;
}
