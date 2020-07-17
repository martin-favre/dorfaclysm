#pragma once
#include <SDL_rect.h>

#include <iostream>
#include "Serializer.h"
class Vector2DInt;
class Vector2D {
 public:
  Vector2D();
  Vector2D(const SerializedObj& serObj);
  Vector2D(double x, double y = 0);
  Vector2D(const Vector2DInt& vec);
  Vector2D& operator+=(const Vector2D&);
  Vector2D& operator-=(const Vector2D&);
  bool operator==(const Vector2D&) const;
  Vector2D operator-(const Vector2D& other) const;
  Vector2D operator+(const Vector2D& other) const;

  /*-------------------------------------------------------
  Returns the angle of the vector in degrees
  (1,0)->0
  (0,1)->90
  ---------------------------------------------------------*/

  static double angle(const Vector2D& vec);
  template <typename multiplierType>
  Vector2D operator*(const multiplierType& value) const;
  template <typename multiplierType>
  Vector2D operator/(const multiplierType& value) const;
  template <typename multiplierType>
  Vector2D& operator/=(const multiplierType& value);
  double magnitude() const;
  double magnitudeSquared() const;
  double dot(const Vector2D& other) const;
  void rotateAround(double angle, Vector2D point);
  void rotate(double angle);
  bool equals(const Vector2D& other, double accuracy);
  /*-------------------------------------------------------
  (x,y) -> (-y,x)
  ---------------------------------------------------------*/
  Vector2D perpendicular() const;
  Vector2D normalized() const;
  void set(double, double);
  void roundToInt();
  SDL_Point getSdlPoint();
  double x = 0;
  double y = 0;
  friend std::ostream& operator<<(std::ostream& stream, const Vector2D&);

 private:
};

void to_json(SerializedObj& j, const Vector2D& vec);
void from_json(const SerializedObj& j, Vector2D& vec);

template <typename multiplierType>
inline Vector2D Vector2D::operator*(const multiplierType& value) const {
  return Vector2D(x * value, y * value);
}

template <typename multiplierType>
inline Vector2D Vector2D::operator/(const multiplierType& value) const {
  return Vector2D(x / value, y / value);
}
template <typename multiplierType>
inline Vector2D& Vector2D::operator/=(const multiplierType& value) {
  x /= value;
  y /= value;
  return *this;
}
