#pragma once
#include "Sprite.h"
#include <map>
#include <memory>
#include <SDL_ttf.h>
class Font;
class SpriteLoader
{
public:
    static std::unique_ptr<Sprite> loadSprite(const std::string &path, const Rect &area);
    static std::unique_ptr<Sprite> loadSprite(const std::string &path);
    static std::unique_ptr<Sprite> getSpriteFromText(const std::string &text, Font &font, SDL_Color &color);
    static std::unique_ptr<Font> loadFont(const std::string &path, int size);
    static void initialize();
    static void teardown();

private:
    SpriteLoader();
    static void loadTexture(const std::string &path);
    static SDL_Texture *convertSurfaceToTexture(SDL_Surface *newSurf);
    static std::map<std::string, SDL_Texture *> mTextures;
    static std::map<std::pair<std::string, int>, TTF_Font *> mFonts;
};