#pragma once
#include "Camera.h"
#include "GraphicsManager.h"
#include "GridMap.h"
namespace GridMapHelpers {
inline void doToEachBlockInScreen(
    const GridMap& map, const Camera& cam,
    std::function<void(const Vector3DInt&)> func) {
  const Vector3DInt cameraPos = cam.getPosition();
  const Vector2DInt tileSize{GridMap::tileRenderSize};
  const Vector3DInt mapSize{map.getSize()};
  const Vector2DInt screenSize{GraphicsManager::getScreenWidth(),
                               GraphicsManager::getScreenHeight()};

  const int numberOfTilesToRenderX = screenSize.x / tileSize.x + 1;
  const int numberOfTilesToRenderY = screenSize.y / tileSize.y + 1;

  const Vector2DInt cameraTilePos = Camera::renderPosToTilePos(cameraPos);
  int endRenderX = cameraTilePos.x + numberOfTilesToRenderX;
  if (endRenderX >= mapSize.x) endRenderX = mapSize.x - 1;

  int endRenderY = cameraTilePos.y + numberOfTilesToRenderY;
  if (endRenderY >= mapSize.y) endRenderY = mapSize.y - 1;
  for (int x = 0; x < numberOfTilesToRenderX; ++x) {
    for (int y = 0; y < numberOfTilesToRenderY; ++y) {
      Vector3DInt pos{x + cameraTilePos.x, y + cameraTilePos.y, cameraPos.z};
      if (!map.isPosInMap(pos)) continue;
      func(pos);
    }
  }
}
}  // namespace GridMap