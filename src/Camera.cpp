#include "Camera.h"

#include "GraphicsManager.h"
#include "GridMap.h"

Camera Camera::mCamera;

Camera& Camera::get() { return mCamera; }
const Vector3DInt& Camera::getPosition() const { return mPosition; }
void Camera::move(const Vector3DInt& movement) {
  mPosition += movement;

  if (mPosition.x < 0) mPosition.x = 0;
  if (mPosition.y < 0) mPosition.y = 0;
  if (mPosition.z < 0) mPosition.z = 0;

  const Vector3DInt gridSize{GridMap::getActiveMap().getSize()};
  const Vector2DInt gridRenderSize{gridSize.x * GridMap::tileRenderSize.x,
                                   gridSize.y * GridMap::tileRenderSize.y};
  const Vector2DInt tilesCameraCovers = {GraphicsManager::getScreenWidth(),
                                         GraphicsManager::getScreenHeight()};

  const Vector2DInt maxPos = gridRenderSize - tilesCameraCovers;
  if (mPosition.x >= maxPos.x) mPosition.x = maxPos.x-1;
  if (mPosition.y >= maxPos.y) mPosition.y = maxPos.y-1;
  if (mPosition.z >= gridSize.z) mPosition.z = gridSize.z-1;
}
const Vector2D& Camera::getScale() const { return mScale; }
void Camera::setScale(const Vector2D& scale) { mScale = scale; }

Vector2DInt Camera::renderPosToTilePos(const Vector2DInt& renderPos) {
  /*
    with tilesize 64
    0 -> 0
    63 -> 0
    64 -> 1
  */

  const int x = round(renderPos.x / GridMap::tileRenderSize.x);
  const int y = round(renderPos.y / GridMap::tileRenderSize.y);
  return Vector2DInt{x, y};
}

template <class T>
bool Camera::inCamera(const T& position) {
  if (position.x < mPosition.x) return false;
  if (position.x >= mPosition.x) return false;
  if (position.y < mPosition.y) return false;
  if (position.y >= mPosition.y) return false;
  return true;
}
