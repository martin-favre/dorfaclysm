#pragma once

#include <hash_fun.h>

#include <cinttypes>
#include <cmath>
#include <functional>
#include <ostream>

#include "Helpers.h"

/*-------------------------------------------------------
        Contains 3D vector of integers
---------------------------------------------------------*/
class Vector2DInt;
class Vector3DInt {
 public:
  Vector3DInt() = default;
  Vector3DInt& operator=(const Vector3DInt& vec) = default;
  Vector3DInt(const Vector3DInt& vec) = default;
  constexpr Vector3DInt(int x_, int y_, int z_ = 0) : x(x_), y(y_), z(z_) {}
  constexpr Vector3DInt(const Vector2DInt& vec);
  // constexpr Vector3DInt(uint x_, uint y_, uint z_) : x(x_), y(y_), z(z_) {}
  // constexpr Vector3DInt(float x_, float y_)
  //     : x(Helpers::roundToInt(x_)), y(Helpers::roundToInt(y_)) {}
  // constexpr Vector3DInt(double x_, double y_)
  //     : x(Helpers::roundToInt(x_)), y(Helpers::roundToInt(y_)) {}

  /*-------------------------------------------------------
          Note: designed for creating an unique value for usage in a map,
          not checking if one is less than the other. Since that doesn't
          sense.
  ---------------------------------------------------------*/
  constexpr bool operator<(const Vector3DInt& left) const {
    if (x != left.x) {
      return x < left.x;
    } else if (y != left.y) {
      return y < left.y;
    } else {
      return z < left.z;
    }
  }

  constexpr bool operator!=(const Vector3DInt& other) const {
    return !(*this == other);
  }

  constexpr bool operator==(const Vector3DInt& other) const {
    return (other.y == y) && (other.x == x) && (other.z == z);
  }

  constexpr Vector3DInt operator*(int val) const {
    return Vector3DInt(x * val, y * val, z * val);
  }

  constexpr Vector3DInt operator+(const Vector3DInt& val) const {
    return Vector3DInt{x + val.x, y + val.y, z + val.z};
  }

  constexpr Vector3DInt operator-(const Vector3DInt& val) const {
    return Vector3DInt{x - val.x, y - val.y, z - val.z};
  }

  constexpr Vector3DInt& operator+=(const Vector3DInt& other) {
    x += other.x;
    y += other.y;
    z += other.z;
    return *this;
  }

  constexpr Vector3DInt& operator-=(const Vector3DInt& other) {
    x -= other.x;
    y -= other.y;
    z -= other.z;
    return *this;
  }

  constexpr int magnitude() const { return sqrt(x * x + y * y + z * z); }
  constexpr int squared_magnitude() const { return x * x + y * y + z * z; }
  friend std::ostream& operator<<(std::ostream& os, const Vector3DInt& vec);

  int x{0};
  int y{0};
  int z{0};
};

#include "Vector2DInt.h"

constexpr Vector3DInt::Vector3DInt(const Vector2DInt& vec)
    : x(vec.x), y(vec.y) {}