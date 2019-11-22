#pragma once
#include "Sprite.h"
#include "ResourceFile.h"
#include <map>
class SpriteLoader
{
    public:
    static std::unique_ptr<Sprite> loadSprite(const ResourceFile* file);
    static std::unique_ptr<Sprite> getSpriteFromText(const std::string & text, TTF_Font* font, SDL_Color& color);
    static void initialize();
    static void teardown();
    private:
    SpriteLoader();
    static SDL_Texture* convertSurfaceToTexture(SDL_Surface* new_surf);
    static std::map<const ResourceFile *, SDL_Texture*> m_textures;
};