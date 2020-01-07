#pragma once
#include <stack>

#include "Vector3DInt.h"
class TileInfo;
class GridMap;
class Astar {
 public:
  /*------------------------------------
  Get path in the grid from a position, to a position.
  --------------------------------------
  Returns:   bool, if it was possible to find a path
  Parameter(out): std::stack<Vector3DInt> & path, will be populated with the
  path Not changed if no good path is found.
  ------------------------------------*/
  bool getPath(const Vector3DInt& from, const Vector3DInt& to,
               const GridMap& map, std::stack<Vector3DInt>& path);

 private:
};