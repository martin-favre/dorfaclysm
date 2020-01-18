#include "GridMapHelpers.h"

#include <stack>

#include "Block.h"
#include "Camera.h"
#include "DeltaPositions.h"
#include "GraphicsManager.h"
#include "GridMap.h"

namespace GridMapHelpers {

void doToEachBlockInScreen(const GridMap& map, const Camera& cam,
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

void exploreMap(GridMap& map, const Vector3DInt& startingFrom) {
  // breadth first search from starting point and set all unexplored tiles to
  // explored if it hits a wall, explore it, but don't continue the search from
  // it.

  const Block& startBlock = map.getBlockAt(startingFrom);
  if (startBlock.isExplored()) return;
  if (!map.isPosFree(startingFrom)) return;
  std::stack<Vector3DInt> unExploredPosses;
  unExploredPosses.push(startingFrom);
  while (!unExploredPosses.empty()) {
    const Vector3DInt current = unExploredPosses.top();
    unExploredPosses.pop();
    for (const Vector3DInt& delta : DELTA_POSITIONS) {
      const Vector3DInt next = current + delta;
      if (map.isPosInMap(next)) {
        Block& block = map.getBlockAt(next);
        if (!block.isExplored()) {
          block.setExplored();
          if (map.isPosFree(next)) {
            unExploredPosses.push(next);
          }
        }
      }
    }
  }
}

}  // namespace GridMapHelpers