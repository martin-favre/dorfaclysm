#pragma once
#include <cinttypes>
#include <cmath>
#include <ostream>

#include "Helpers.h"
#include "Vector2D.h"
/*-------------------------------------------------------
        Contains 2D vector of integers
---------------------------------------------------------*/

class Vector2DInt {
 public:
  Vector2DInt() = default;
  constexpr Vector2DInt(int x_, int y_) : x(x_), y(y_) {}
  constexpr Vector2DInt(uint x_, uint y_) : x(x_), y(y_) {}

  /*-------------------------------------------------------
  Values are rounded.
  ---------------------------------------------------------*/
  constexpr Vector2DInt(float x_, float y_)
      : x(Helpers::roundToInt(x_)), y(Helpers::roundToInt(y_)) {}
  constexpr Vector2DInt(double x_, double y_)
      : x(Helpers::roundToInt(x_)), y(Helpers::roundToInt(y_)) {}
  constexpr Vector2DInt(const Vector2D& vec)
      : x(Helpers::roundToInt(vec.x)), y(Helpers::roundToInt(vec.y)) {}

  /*-------------------------------------------------------
          Note: designed for creating an unique value for usage in a map,
          not checking if one is less than the other. Since that doesn't
          sense.
  ---------------------------------------------------------*/
  bool operator<(const Vector2DInt& left) const;

  bool operator!=(const Vector2DInt& other) const;
  bool operator==(const Vector2DInt& other) const;
  Vector2DInt operator*(const int& val);
  Vector2DInt& operator+=(const Vector2DInt& other);
  int x = 0;
  int y = 0;
  friend std::ostream& operator<<(std::ostream& os, const Vector2DInt& vec);
};
