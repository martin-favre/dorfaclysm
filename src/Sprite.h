#pragma once
#include <SDL.h>

#include <string>

#include "Rect.h"
class Sprite {
 public:
  Sprite(SDL_Texture* newText, bool isFont = false);
  Sprite(SDL_Texture* newText, const Rect& area, bool isFont = false);
  ~Sprite();
  SDL_Texture* getSdlTexture() const;
  const Rect& getRect() const;

 private:
  SDL_Texture* mTexture{nullptr};
  Rect mRect;
  const bool mIsFont{false};
};