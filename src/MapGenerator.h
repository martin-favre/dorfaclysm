#pragma once
#include <memory>

#include "GridMap.h"
class Vector3DInt;
class MapGenerator {
 public:
  static void generate(GridMap& gridMap, const Vector3DInt& size);

 private:
  static void generateFlatWorld(GridMap& gridMap, const Vector3DInt& size);
  static void generateStairWorld(GridMap& gridMap, const Vector3DInt& size);
  static void generateTwoLevelWorld(GridMap& gridMap, const Vector3DInt& size);
};