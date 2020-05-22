#pragma once
#include <SDL.h>
#include <SDL_image.h>

#include <memory>

#include "Component.h"
#include "SpriteSheetInfo.h"

#include <mutex>
class Rect;
class Sprite;
class GameObject;
class SpriteComponent : public Component {
 public:
  SpriteComponent(GameObject& owner, const SpriteSheetInfo& spriteSheet,
                  const Vector2DInt& indexInSpritesheet);
  SpriteComponent(GameObject& owner, const SerializedObj& serObj);
  void setup() override;
  void render() override;
  void teardown() override;
  void setCameraAsReference(bool useCamera) {
    std::scoped_lock lock(mMutex);
    mCameraAsReference = useCamera;
  }

  SerializedObj serialize() const override;
  static std::string getTypeString() { return "SpriteComponent";}
 private:
  bool mCameraAsReference{true};
  bool mScaleToTileGrid{true};
  SpriteSheetInfo mSpriteSheetInfo;
  Vector2DInt mSpriteSheetIndex;

  const bool mCentered{false};
  std::unique_ptr<Sprite> mSprite;
  const SDL_RendererFlip mFlip{SDL_FLIP_NONE};
  std::mutex mMutex;
};
