class Point {
 private:
  double x;
  double y;

 public:
  Point();
  void move(double x, double y);
  double distanceFrom(const Point & p);
};
