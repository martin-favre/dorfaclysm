#pragma once
#include <SDL.h>
#include <SDL_image.h>

#include <memory>

#include "Component.h"

class Rect;
class Sprite;
class GameObject;
class SpriteComponent : public Component {
 public:
  SpriteComponent(GameObject &owner, std::unique_ptr<Sprite> &&sprite);
  void loadSpriteFromImage(const std::string &path);
  void loadSpriteFromImage(const std::string &path, const Rect &area);
  void render() override;
  void setCameraAsReference(bool useCamera) { mCameraAsReference = useCamera; }

 private:
  std::unique_ptr<Sprite> mSprite;
  bool mCameraAsReference{true};
  bool mScaleToTileGrid{true};
  bool mCentered{false};
  SDL_RendererFlip mFlip{SDL_FLIP_NONE};
};
