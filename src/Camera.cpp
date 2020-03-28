#include "Camera.h"

#include "GraphicsManager.h"
#include "GridMap.h"

Camera Camera::mCamera;

Camera::Camera(){
  // Camera topleft corner needs to be in topleft corner
  
}

Camera& Camera::get() { return mCamera; }
Vector3DInt Camera::getPosition() const {
  std::scoped_lock lock(mMutex);
  return mPosition;
}
void Camera::move(const Vector3DInt& movement) {
  std::scoped_lock lock(mMutex);
  mPosition += movement;

  if (mPosition.x < 0) mPosition.x = 0;
  if (mPosition.y < 0) mPosition.y = 0;
  if (mPosition.z < 0) mPosition.z = 0;

  const Vector3DInt gridSize{GridMap::getActiveMap().getSize()};
  const Vector2DInt gridRenderSize{gridSize.x * tileRenderSize.x,
                                   gridSize.y * tileRenderSize.y};
  const Vector2DInt tilesCameraCovers = {GraphicsManager::getScreenWidth(),
                                         GraphicsManager::getScreenHeight()};

  const Vector2DInt maxPos = gridRenderSize - tilesCameraCovers;
  if (mPosition.x >= maxPos.x) mPosition.x = maxPos.x - 1;
  if (mPosition.y >= maxPos.y) mPosition.y = maxPos.y - 1;
  if (mPosition.z >= gridSize.z) mPosition.z = gridSize.z - 1;
}
Vector2D Camera::getScale() const {
  std::scoped_lock lock(mMutex);
  return mScale;
}
void Camera::setScale(const Vector2D& scale) {
  std::scoped_lock lock(mMutex);
  mScale = scale;
}

Vector3DInt Camera::renderPosToTilePos(const Vector3DInt& renderPos) {
  /*
    with tilesize 64
    0 -> 0
    63 -> 0
    64 -> 1
  */

  const int x = round(renderPos.x / tileRenderSize.x);
  const int y = round(renderPos.y / tileRenderSize.y);
  return Vector3DInt{x, y, renderPos.z};
}

Vector2DInt Camera::tilePosToRenderPos(const Vector2DInt& pos) {
  return Vector2DInt{pos.x * tileRenderSize.x,
                     pos.y * tileRenderSize.y};
}

template <class T>
bool Camera::inCamera(const T& position) {
  std::scoped_lock lock(mMutex);
  if (position.x < mPosition.x) return false;
  if (position.x >= mPosition.x) return false;
  if (position.y < mPosition.y) return false;
  if (position.y >= mPosition.y) return false;
  return true;
}
