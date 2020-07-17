#pragma once
#include "Vector3DInt.h"
class Vector3DIntHash {
 public:
  inline size_t operator()(const Vector3DInt& vec) const {
    constexpr int random_prime_1 = 7919;
    constexpr int random_prime_2 = 6599;
    return (vec.x * random_prime_1 + vec.y) * random_prime_2 + vec.z;
  }
};
