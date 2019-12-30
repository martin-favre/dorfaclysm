#pragma once
#include "Vector2D.h"
#include "Vector2DInt.h"
class Camera {
 public:
  Camera(const Camera&) = delete;
  Camera& operator=(const Camera&) = delete;

  static Camera& get();
  const Vector2DInt& getPosition() const;
  void move(const Vector2DInt& movement);
  const Vector2D& getScale() const;
  void setScale(const Vector2D& scale);

  template <class T>
  bool inCamera(const T& position);

  static Vector2DInt renderPosToTilePos(const Vector2DInt& renderPos);

 private:
  Camera() = default;
  static Camera mCamera;
  Vector2DInt mPosition;
  Vector2D mScale{1, 1};
};