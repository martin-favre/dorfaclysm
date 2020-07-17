#include "Vector3DInt.h"

std::ostream& operator<<(std::ostream& os, const Vector3DInt& dt) {
  os << "(" << dt.x << "," << dt.y << "," << dt.z << ")";
  return os;
}