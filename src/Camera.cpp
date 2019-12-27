#include "Camera.h"
#include "GridMap.h"
#include "GraphicsManager.h"

Camera Camera::mCamera;

Camera& Camera::get() { return mCamera; }
const Vector2D& Camera::getPosition() const { return mPosition; }
void Camera::move(const Vector2D& movement) { 
    mPosition += movement; 

    if(mPosition.x < 0) mPosition.x = 0;
    if(mPosition.y < 0) mPosition.y = 0;
    int outerBoundsX = mPosition.x + GraphicsManager::getScreenWidth();
    int outerBoundsY = mPosition.y + GraphicsManager::getScreenHeight();
    Vector2DInt gridSize{GridMap::getActiveMap().getSize()};
    Vector2DInt gridRenderWidth{gridSize.x*GridMap::tileRenderSize.x, gridSize.y*GridMap::tileRenderSize.y};
    if(outerBoundsX >= gridRenderWidth.x) mPosition += outerBoundsX - gridRenderWidth.x;
    if(outerBoundsY >= gridRenderWidth.y) mPosition += outerBoundsY - gridRenderWidth.y;
  }
const Vector2D& Camera::getScale() const { return mScale; }
void Camera::setScale(const Vector2D& scale) { mScale = scale; }


Vector2DInt Camera::renderPosToTilePos(const Vector2D& renderPos)
{
  /*
    with tilesize 64
    0 -> 0
    63 -> 0
    64 -> 1
  */

  const int x = round(renderPos.x / GridMap::tileRenderSize.x);
  const int y = round(renderPos.y / GridMap::tileRenderSize.y);
  return Vector2DInt{x,y};
}

template <class T>
bool Camera::inCamera(const T& position) {
  if (position.x < mPosition.x) return false;
  if (position.x >= mPosition.x) return false;
  if (position.y < mPosition.y) return false;
  if (position.y >= mPosition.y) return false;
  return true;
}
