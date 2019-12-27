#pragma once
#include "GridMap.h"
#include <memory>
class Vector2DInt;
class MapGenerator
{
    public:
    static void generate(GridMap& gridMap, const Vector2DInt& size);
};