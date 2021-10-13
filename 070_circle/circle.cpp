#include "circle.hpp"

#include <cmath>
#include <cstdio>

Circle::Circle(Point center_init, double radius_init) :
    center(center_init),
    radius(radius_init) {
}

void Circle::move(double dx, double dy) {
  this->center.move(dx, dy);
}

double Circle::intersectionArea(const Circle & otherCircle) {
  double r1 = this->radius;
  double r2 = otherCircle.radius;
  double d = this->center.distanceFrom(otherCircle.center);
  if ((r1 + r2) <= d) {
    return 0;
  }
  else if (r1 < r2 && d <= (r2 - r1)) {
    return M_PI * r1 * r1;
  }
  else if (r1 >= r2 && d <= (r1 - r2)) {
    return M_PI * r2 * r2;
  }
  else {
    double d1 = (r1 * r1 - r2 * r2 + d * d) * d / 2;
    double d2 = d - d1;
    return r1 * r1 * acos(d1 / r1) + r2 * r2 * acos(d2 / r2) -
           d1 * sqrt(r1 * r1 - d1 * d1) - d2 * sqrt(r2 * r2 - d2 * d2);
  }
}
