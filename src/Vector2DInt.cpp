#include "Vector2DInt.h"

#include "Vector2D.h"

bool Vector2DInt::operator<(const Vector2DInt& left) const {
  if (x != left.x) {
    return x < left.x;
  } else {
    return y < left.y;
  }
}

bool Vector2DInt::operator!=(const Vector2DInt& other) const {
  return !(*this == other);
}

bool Vector2DInt::operator==(const Vector2DInt& other) const {
  return (other.y == y) && (other.x == x);
}
Vector2DInt Vector2DInt::operator*(const int& val) const {
  return Vector2DInt(x * val, y * val);
}

Vector2DInt Vector2DInt::operator-(const Vector2DInt& val) const {
  return Vector2DInt{x - val.x, y - val.y};
}

Vector2DInt& Vector2DInt::operator+=(const Vector2DInt& other) {
  x += other.x;
  y += other.y;
  return *this;
}
Vector2DInt& Vector2DInt::operator-=(const Vector2DInt& other) {
  x -= other.x;
  y -= other.y;
  return *this;
}

std::ostream& operator<<(std::ostream& os, const Vector2DInt& dt) {
  os << "(" << dt.x << "," << dt.y << ")";
  return os;
}

void to_json(SerializedObj& j, const Vector2DInt& vec) {
  j = {{"x", vec.x}, {"y", vec.y}};
}

void from_json(const SerializedObj& j, Vector2DInt& vec) {
  j.at("x").get_to(vec.x);
  j.at("y").get_to(vec.y);
}
