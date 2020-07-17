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
                                 const SpriteSheetInfo& spriteSheet,
                                 const Vector2DInt& indexInSpritesheet)
    : Component(owner),
      mSpriteSheetInfo(spriteSheet),
      mSpriteSheetIndex(indexInSpritesheet) {}

SpriteComponent::SpriteComponent(GameObject& owner, const SerializedObj& serObj)
    : Component(owner, serObj.at(SerializeString_Parent)),
      mCameraAsReference(serObj.at("cameraAsReference")),
      mScaleToTileGrid(serObj.at("scaleToGrid")),
      mSpriteSheetInfo(serObj.at("spritesheetInfo")),
      mSpriteSheetIndex(serObj.at("spriteSheetIndex")) {}

SerializedObj SpriteComponent::serialize() const {
  SerializedObj out = createSerializedObj<SpriteComponent>();
  out["cameraAsReference"] = mCameraAsReference;
  out["scaleToGrid"] = mScaleToTileGrid;
  out["spritesheetInfo"] = mSpriteSheetInfo;
  out["spriteSheetIndex"] = mSpriteSheetIndex;
  out[SerializeString_Parent] = Component::serialize();
  return out;
}

void SpriteComponent::setup() {
  mSprite =
      SpriteLoader::loadSpriteByIndex(mSpriteSheetInfo, mSpriteSheetIndex);
}

void SpriteComponent::teardown() { std::scoped_lock lock(mMutex); }

void SpriteComponent::render() {
  std::scoped_lock lock(mMutex);
  if (mSprite) {
    const Vector3DInt& camPos = Camera::get().getPosition();
    Vector3DInt pos{owner().getPosition()};
    if (mScaleToTileGrid) {
      pos.x *= Camera::tileRenderSize.x;
      pos.y *= Camera::tileRenderSize.y;
      const Vector2D& scale = owner().getScale();
      const int centreOffsetX =
          Camera::tileRenderSize.x / 2 - (scale.x * mSprite->getRect().w()) / 2;
      const int centreOffsetY =
          Camera::tileRenderSize.y / 2 - (scale.y * mSprite->getRect().h()) / 2;
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