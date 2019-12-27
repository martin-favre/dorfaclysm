#include "Sprite.h"

#include <SDL_image.h>

#include "Helpers.h"
#include "Vector2DInt.h"

Sprite::Sprite(SDL_Texture *newText, const Rect &area, bool isFont)
    : mTexture(newText), mRect(area), mIsFont(isFont) {}

Sprite::Sprite(SDL_Texture *newText, bool isFont)
    : mTexture(newText), mIsFont(isFont) {
  int width = 0;
  int height = 0;
  SDL_QueryTexture(mTexture, NULL, NULL, &width, &height);
  mRect = Rect{0, 0, width, height};
}

Sprite::~Sprite() {
  if (mIsFont) {
    // Since text textures can't be cached
    // We have to close the texture
    ASSERT(mTexture, "Is not supposed to be null");
    SDL_DestroyTexture(mTexture);
  }
}

SDL_Texture *Sprite::getSdlTexture() const { return mTexture; }

const Rect &Sprite::getRect() const { return mRect; }
