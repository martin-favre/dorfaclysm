#include "Vector2D.h"

#include "Helpers.h"
#include "Vector2DInt.h"
Vector2D::Vector2D() {}
Vector2D::Vector2D(double _x, double _y) : x(_x), y(_y) {}
Vector2D::Vector2D(const Vector2DInt& vec) : x(vec.x), y(vec.y) {}
Vector2D::Vector2D(const SerializedObj& serObj) { from_json(serObj, *this); }

Vector2D& Vector2D::operator+=(const Vector2D& other) {
  x += other.x;
  y += other.y;
  return *this;
}

Vector2D& Vector2D::operator-=(const Vector2D& other) {
  x -= other.x;
  y -= other.y;
  return *this;
}

bool Vector2D::operator==(const Vector2D& other) const {
  return x == other.x && y == other.y;
}

Vector2D Vector2D::operator-(const Vector2D& other) const {
  return Vector2D(x - other.x, y - other.y);
}

Vector2D Vector2D::operator+(const Vector2D& other) const {
  return Vector2D(x + other.x, y + other.y);
}

double Vector2D::angle(const Vector2D& vec) {
  double ret = atanf(vec.y / vec.x) / Helpers::TO_RAD;
  if (vec.x < 0 && vec.y < 0)  // quadrant Ⅲ
    ret = 180 + ret;
  else if (vec.x < 0)        // quadrant Ⅱ
    ret = 180 + ret;         // it actually substracts
  else if (vec.y < 0)        // quadrant Ⅳ
    ret = 270 + (90 + ret);  // it actually substracts
  return ret;
}

double Vector2D::magnitude() const { return sqrt(x * x + y * y); }

double Vector2D::magnitudeSquared() const { return x * x + y * y; }

double Vector2D::dot(const Vector2D& other) const {
  return x * other.x + y * other.y;
}

void Vector2D::rotateAround(double angle, const Vector2D pivot_point) {
  angle = Helpers::TO_RAD * angle;
  const double angle_sin = sin(angle);
  const double angle_cos = cos(angle);
  x -= pivot_point.x;
  y -= pivot_point.y;
  const double xnew = x * angle_cos - y * angle_sin;
  const double ynew = x * angle_sin + y * angle_cos;
  x = xnew + pivot_point.x;
  y = ynew + pivot_point.y;
}

void Vector2D::rotate(double angle) {
  angle = Helpers::TO_RAD * angle;
  double oldX = x;
  x = x * cos(angle) + y * sin(angle);
  y = y * cos(angle) - oldX * sin(angle);
}

bool Vector2D::equals(const Vector2D& other, double accuracy) {
  const bool same_x = abs(other.x - x) < accuracy;
  const bool same_y = abs(other.y - y) < accuracy;
  return same_x && same_y;
}

Vector2D Vector2D::perpendicular() const { return Vector2D(-y, x); }

Vector2D Vector2D::normalized() const {
  const double magn = magnitude();
  return Vector2D(x / magn, y / magn);
}

void Vector2D::set(double _x, double _y) {
  x = _x;
  y = _y;
}

void Vector2D::roundToInt() {
  x = double(round(x));
  y = double(round(y));
}

SDL_Point Vector2D::getSdlPoint() {
  return SDL_Point{int(round(x)), int(round(y))};
}
std::ostream& operator<<(std::ostream& stream, const Vector2D& p) {
  stream << "(" << p.x << "," << p.y << ")";
  return stream;
}

void to_json(SerializedObj& j, const Vector2D& vec) {
  j = {{"x", vec.x}, {"y", vec.y}};
}
void from_json(const SerializedObj& j, Vector2D& vec) {
  j.at("x").get_to(vec.x);
  j.at("y").get_to(vec.y);
}
