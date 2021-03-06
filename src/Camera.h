#pragma once
#include "Vector2D.h"
#include "Vector2DInt.h"
#include <mutex>
class Camera {
 public:
  Camera(const Camera&) = delete;
  Camera& operator=(const Camera&) = delete;

  static Camera& get();
  Vector3DInt getPosition() const;
  void move(const Vector3DInt& movement);
  Vector2D getScale() const;
  void setScale(const Vector2D& scale);

  template <class T>
  bool inCamera(const T& position);

  static Vector3DInt renderPosToTilePos(const Vector3DInt& renderPos);
  static Vector2DInt tilePosToRenderPos(const Vector2DInt& pos);
  static constexpr Vector2DInt tileRenderSize{
      16, 16};  // How many pixels wide a tile is when rendered, before other
                // modifiers
 private:
  Camera();
  static Camera mCamera;
  Vector3DInt mPosition;
  Vector2D mScale{1, 1};
  mutable std::mutex mMutex;
};