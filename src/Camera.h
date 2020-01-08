#pragma once
#include "Vector2D.h"
#include "Vector2DInt.h"
class Camera {
 public:
  Camera(const Camera&) = delete;
  Camera& operator=(const Camera&) = delete;

  static Camera& get();
  const Vector3DInt& getPosition() const;
  void move(const Vector3DInt& movement);
  const Vector2D& getScale() const;
  void setScale(const Vector2D& scale);

  template <class T>
  bool inCamera(const T& position);

  static Vector3DInt renderPosToTilePos(const Vector3DInt& renderPos);

 private:
  Camera() = default;
  static Camera mCamera;
  Vector3DInt mPosition;
  Vector2D mScale{1, 1};
};