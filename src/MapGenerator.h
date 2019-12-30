#pragma once
#include <memory>

#include "GridMap.h"
class Vector2DInt;
class MapGenerator {
 public:
  static void generate(GridMap& gridMap, const Vector2DInt& size);

 private:
  static void generateFloor(GridMap& gridMap, const Vector2DInt& size);
  static void generateRocks(GridMap& gridMap, const Vector2DInt& size);
};