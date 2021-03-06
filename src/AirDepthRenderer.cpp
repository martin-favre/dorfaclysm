#include "AirDepthRenderer.h"

#include "Block.h"
#include "Camera.h"
#include "Component.h"
#include "GraphicsManager.h"
#include "GridMap.h"
#include "GridMapHelpers.h"
#include "Serializer.h"

void AirDepthRenderer::renderBlock(Vector3DInt pos) {
  int depth = 0;
  constexpr uint8_t maxDepth = 6;
  if (!mGridMap.getBlockAt(pos).isExplored()) return;
  while (!mGridMap.getBlockAt(pos).getTopSprite()) {
    pos += {0, 0, -1};
    ++depth;
    if (depth >= maxDepth) break;
  }
  if (depth == 0) return;
  uint8_t alpha = depth * SDL_ALPHA_OPAQUE / maxDepth;
  GraphicsManager::setRenderDrawColor({0, 0, 20, alpha});
  Vector3DInt renderPos = Camera::tilePosToRenderPos(pos);
  renderPos -= mCam.getPosition();
  const Rect dstRect{renderPos.x, renderPos.y, Camera::tileRenderSize.x,
                     Camera::tileRenderSize.y};

  GraphicsManager::drawRect(dstRect);
  GraphicsManager::setRenderDrawColor(GraphicsManager::mDefaultDrawColor);
}

AirDepthRenderer::AirDepthRenderer(GameObject& gObj,
                                   const SerializedObj& serObj)
    : Component(gObj, serObj["parent"]),
      mGridMap(GridMap::getActiveMap()),
      mCam(Camera::get()) {}

SerializedObj AirDepthRenderer::serialize() const {
  SerializedObj out = createSerializedObj<AirDepthRenderer>();
  out["parent"] = Component::serialize();
  return out;
}

AirDepthRenderer::AirDepthRenderer(GameObject& gObj)
    : Component(gObj), mGridMap(GridMap::getActiveMap()), mCam(Camera::get()) {}

void AirDepthRenderer::render() {
  std::function<void(const Vector3DInt&)> foo = [this](const Vector3DInt& pos) {
    this->renderBlock(pos);
  };
  GridMapHelpers::doToEachBlockInScreen(mGridMap, mCam, foo);
}

std::string AirDepthRenderer::getTypeString() { return "AirDepthRenderer"; }