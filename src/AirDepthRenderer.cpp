#include "AirDepthRenderer.h"

#include "Block.h"
#include "Camera.h"
#include "GraphicsManager.h"
#include "GridMap.h"

void renderAirAt(Vector3DInt pos, const GridMap& gridmap, const Rect& dstRect) {
  int depth = 0;
  constexpr uint8_t maxDepth = 20;
  while (gridmap.getBlockAt(pos).isSeeThrough()) {
    pos += {0, 0, -1};
    ++depth;
    if (depth >= maxDepth) break;
  }
  if (depth == 0) return;
  uint8_t alpha = depth * SDL_ALPHA_OPAQUE / maxDepth;
  GraphicsManager::setRenderDrawColor({0, 0, 255, alpha});
  GraphicsManager::drawRect(dstRect);
  GraphicsManager::setRenderDrawColor(GraphicsManager::mDefaultDrawColor);
}

AirDepthRenderer::AirDepthRenderer(GameObject& gObj)
    : Component(gObj), mGridMap(GridMap::getActiveMap()), mCam(Camera::get()) {}

void AirDepthRenderer::render() {
  const Vector3DInt cameraPos = mCam.getPosition();
  const Vector2DInt tileSize{GridMap::tileRenderSize};
  const Vector3DInt mapSize{mGridMap.getSize()};
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
      if (!mGridMap.isPosInMap(pos)) continue;
      const int renderPosX = GridMap::tileRenderSize.x * x;
      const int renderPosY = GridMap::tileRenderSize.y * y;
      const Rect dstRect{renderPosX, renderPosY, GridMap::tileRenderSize.x,
                         GridMap::tileRenderSize.y};

      renderAirAt(pos, mGridMap, dstRect);
    }
  }
}