#pragma once
#include <stack>

#include "Vector2DInt.h"
class TileInfo;
class GridMap;
class Astar {
 public:
  /*------------------------------------
  Get path in the grid from a position, to a position.
  --------------------------------------
  Returns:   bool, if it was possible to find a path
  Parameter(out): std::stack<Vector2DInt> & path, will be populated with the
  path Not changed if no good path is found.
  ------------------------------------*/
  bool getPath(const Vector2DInt& from, const Vector2DInt& to,
               const GridMap& map, std::stack<Vector2DInt>& path);

 private:
};