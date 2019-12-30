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

void SpriteComponent::render() {
  if (mSprite) {
    const auto& camPos = Camera::get().getPosition();
    Vector2DInt pos{owner().getPosition()};
    if (mScaleToTileGrid) {
      pos.x *= GridMap::tileRenderSize.x;
      pos.y *= GridMap::tileRenderSize.y;
    }
    if (mCameraAsReference) {
      pos -= camPos;
    }
    GraphicsManager::renderTexture(*mSprite, pos, owner().getScale(),
                                   owner().getRotation(), mCentered, mFlip);
  }
}