#pragma once
#include "Vector2DInt.h"
#include "Vector2D.h"
class Camera
{
  public:
  Camera(const Camera&) = delete;
  Camera& operator=(const Camera&) = delete;

  static Camera& get();
  const Vector2D& getPosition()const;
  void move(const Vector2D& movement);
  const Vector2D& getScale() const;
  void setScale(const Vector2D& scale);

  template<class T>
  bool inCamera(const T& position);

  static Vector2DInt renderPosToTilePos(const Vector2D& renderPos);

  private:
  Camera() = default;
  static Camera mCamera;
  Vector2D mPosition;
  Vector2D mScale{1,1};
};