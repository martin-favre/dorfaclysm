#pragma once
#include <functional>

#include "Vector3DInt.h"

class GridMap;
class Camera;
namespace GridMapHelpers {
void doToEachBlockInScreen(const GridMap& map, const Camera& cam,
                           std::function<void(const Vector3DInt&)> func);
void exploreMap(GridMap& map, const Vector3DInt& startingFrom);
bool getClosestFreePositionTo(const GridMap& map, const Vector3DInt pos,
                              Vector3DInt& out, uint widthToSearch = 1, uint minWidth = 0);

}  // namespace GridMapHelpers