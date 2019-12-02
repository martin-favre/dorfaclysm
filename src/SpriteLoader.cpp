#include "SpriteLoader.h"

#include "Font.h"
#include "GraphicsManager.h"
#include "Helpers.h"
#include "Logging.h"
#include "Rect.h"

std::map<std::string, SDL_Texture *> SpriteLoader::mTextures;
std::map<std::pair<std::string, int>, TTF_Font *> SpriteLoader::mFonts;

SDL_Texture *SpriteLoader::convertSurfaceToTexture(SDL_Surface *newSurf) {
  SDL_Texture *out =
      SDL_CreateTextureFromSurface(GraphicsManager::mMainRenderer, newSurf);
  ASSERT(out,
         "Could not load texture, SDL_Error: " + std::string(SDL_GetError()));
  return out;
}

std::unique_ptr<Sprite> SpriteLoader::getSpriteFromText(const std::string &text,
                                                        Font &font,
                                                        SDL_Color &textColor) {
  SDL_Surface *newSurf =
      TTF_RenderText_Solid(font.getSdlFont(), text.c_str(), textColor);
  SDL_Texture *newText = convertSurfaceToTexture(newSurf);
  SDL_FreeSurface(newSurf);

  // The responsibility of cleanup goes to the sprite,
  // Since we can't cache font textures.
  return std::make_unique<Sprite>(newText, true);
}

void SpriteLoader::teardown() {
  for (const auto &item : mTextures) {
    SDL_DestroyTexture(item.second);
  }
}

void SpriteLoader::loadTexture(const std::string &path) {
  SDL_Surface *newSurf = IMG_Load(path.c_str());
  ASSERT(newSurf,
         "Could not load image " + path + "IMG_GetError: " + IMG_GetError());
  SDL_Texture *newText = SpriteLoader::convertSurfaceToTexture(newSurf);
  mTextures[path] = newText;
}

std::unique_ptr<Font> SpriteLoader::loadFont(const std::string &path,
                                             int size) {
  const std::pair<std::string, int> pair(path, size);
  if (!mFonts.count(pair)) {
    mFonts[pair] = TTF_OpenFont(path.c_str(), size);
    ASSERT(mFonts[pair],
           "Could not open font " + path + "TTF_GetError:" + TTF_GetError());
  }
  return std::make_unique<Font>(mFonts[pair], size);
}

std::unique_ptr<Sprite> SpriteLoader::loadSprite(const std::string &path) {
  if (!mTextures.count(path)) {
    loadTexture(path);
  }
  return std::make_unique<Sprite>(mTextures[path]);
}
std::unique_ptr<Sprite> SpriteLoader::loadSprite(const std::string &path,
                                                 const Rect &area) {
  if (!mTextures.count(path)) {
    loadTexture(path);
  }
  return std::make_unique<Sprite>(mTextures[path], area);
}
