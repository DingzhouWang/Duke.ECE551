#include "point.hpp"
class Circle {
  Point center;
  const double radius;

 public:
  Circle(Point center_init, double radius_init);
  void move(double dx, double dy);
  double intersectionArea(const Circle & otherCircle);
};
