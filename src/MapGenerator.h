#pragma once
#include <memory>

#include "GridMap.h"
class Vector3DInt;
class MapGenerator {
 public:
  static void generate(GridMap& gridMap, const Vector3DInt& size);

 private:

};