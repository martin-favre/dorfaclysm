#pragma once
#include <SDL.h>
#include <SDL_image.h>

#include <memory>

#include "Component.h"
#include <mutex>
class Rect;
class Sprite;
class GameObject;
class SpriteComponent : public Component {
 public:
  SpriteComponent(GameObject &owner, std::unique_ptr<Sprite> &&sprite);
  void render() override;
  void teardown() override;
  void setCameraAsReference(bool useCamera) { 
    std::scoped_lock lock(mMutex);
    mCameraAsReference = useCamera; }

 private:
  const std::unique_ptr<Sprite> mSprite;
  bool mCameraAsReference{true};
  bool mScaleToTileGrid{true};
  const bool mCentered{false};
  const SDL_RendererFlip mFlip{SDL_FLIP_NONE};
  std::mutex mMutex;
};
