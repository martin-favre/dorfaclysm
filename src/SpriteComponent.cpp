#include "SpriteComponent.h"

#include <string>

#include "Camera.h"
#include "GameObject.h"
#include "GraphicsManager.h"
#include "GridMap.h"
#include "Rect.h"
#include "Sprite.h"
#include "SpriteLoader.h"
#include "Vector2D.h"
SpriteComponent::SpriteComponent(GameObject& owner,
                                 std::unique_ptr<Sprite>&& sprite)
    : Component(owner), mSprite(std::move(sprite)) {}

void SpriteComponent::teardown() {
  std::scoped_lock lock(mMutex);
}

void SpriteComponent::render() {
  std::scoped_lock lock(mMutex);
  if (mSprite) {
    const Vector3DInt& camPos = Camera::get().getPosition();
    Vector3DInt pos{owner().getPosition()};
    if (mScaleToTileGrid) {
      pos.x *= GridMap::tileRenderSize.x;
      pos.y *= GridMap::tileRenderSize.y;
      const Vector2D& scale = owner().getScale();
      const int centreOffsetX = GridMap::tileRenderSize.x / 2 -
                                (scale.x * mSprite->getRect().w()) / 2;
      const int centreOffsetY = GridMap::tileRenderSize.y / 2 -
                                (scale.y * mSprite->getRect().h()) / 2;
      pos.x += centreOffsetX;
      pos.y += centreOffsetY;
    }
    if (mCameraAsReference) {
      if (pos.z != camPos.z) return;
      pos -= camPos;
    }
    GraphicsManager::renderTexture(*mSprite, pos, owner().getScale(),
                                   owner().getRotation(), mCentered, mFlip);
  }
}