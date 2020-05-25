#include "Vector3DInt.h"

Vector3DInt::Vector3DInt(const SerializedObj& serObj) { from_json(serObj, *this); }

std::ostream& operator<<(std::ostream& os, const Vector3DInt& dt) {
  os << "(" << dt.x << "," << dt.y << "," << dt.z << ")";
  return os;
}

void to_json(SerializedObj& j, const Vector3DInt& vec) {
  j = {{"x", vec.x}, {"y", vec.y}, {"z", vec.z}};
}

void from_json(const SerializedObj& j, Vector3DInt& vec) {
  j.at("x").get_to(vec.x);
  j.at("y").get_to(vec.y);
  j.at("z").get_to(vec.z);
}
