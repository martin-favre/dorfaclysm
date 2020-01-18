#include "AirDepthRenderer.h"

#include "Block.h"
#include "Camera.h"
#include "GraphicsManager.h"
#include "GridMapHelpers.h"
#include "GridMap.h"

void AirDepthRenderer::renderBlock(Vector3DInt pos) {
  int depth = 0;
  constexpr uint8_t maxDepth = 20;
  while (mGridMap.getBlockAt(pos).isSeeThrough()) {
    pos += {0, 0, -1};
    ++depth;
    if (depth >= maxDepth) break;
  }
  if (depth == 0) return;
  uint8_t alpha = depth * SDL_ALPHA_OPAQUE / maxDepth;
  GraphicsManager::setRenderDrawColor({0, 0, 255, alpha});
  Vector3DInt renderPos = Camera::tilePosToRenderPos(pos);
  renderPos -= mCam.getPosition();
  const Rect dstRect{renderPos.x, renderPos.y, GridMap::tileRenderSize.x,
                     GridMap::tileRenderSize.y};

  GraphicsManager::drawRect(dstRect);
  GraphicsManager::setRenderDrawColor(GraphicsManager::mDefaultDrawColor);
}

AirDepthRenderer::AirDepthRenderer(GameObject& gObj)
    : Component(gObj), mGridMap(GridMap::getActiveMap()), mCam(Camera::get()) {}

void AirDepthRenderer::render() {
  std::function<void(const Vector3DInt&)> foo = [this](const Vector3DInt& pos) {
    this->renderBlock(pos);
  };
  GridMapHelpers::doToEachBlockInScreen(mGridMap, mCam, foo);
}