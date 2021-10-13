#include "point.hpp"

#include <cmath>

Point::Point() : x(0), y(0) {
}

void Point::move(double dx, double dy) {
  this->x += dx;
  this->y += dy;
}

double Point::distanceFrom(const Point & p) {
  double a = p.x - this->x;
  double b = p.y - this->y;
  return sqrt(pow(a, 2) + pow(b, 2));
}
