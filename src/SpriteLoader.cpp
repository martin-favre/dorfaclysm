#include "SpriteLoader.h"
#include "GraphicsManager.h"
#include "Logging.h"


std::map<const ResourceFile *, SDL_Texture*> SpriteLoader::m_textures;

SDL_Texture* SpriteLoader::convertSurfaceToTexture(SDL_Surface* new_surf) {
	SDL_Texture* out = SDL_CreateTextureFromSurface(GraphicsManager::mMainRenderer, new_surf);
	ASSERT(out, "Could not load texture, SDL_Error: " + std::string(SDL_GetError()));
	return out;
}

std::unique_ptr<Sprite> SpriteLoader::getSpriteFromText(const std::string & text, TTF_Font * font , SDL_Color & textColor) {
	SDL_Surface* new_surf = TTF_RenderText_Solid(font, text.c_str(), textColor);
	SDL_Texture* new_text = convertSurfaceToTexture(new_surf);
	SDL_FreeSurface(new_surf);

    // The responsibility of cleanup goes to the sprite,
    // Since we can't cache font textures.
	return std::make_unique<Sprite>(new_text, true);
}

void SpriteLoader::teardown()
{
    for (const auto& item : m_textures)
    {
        SDL_DestroyTexture(item.second);
    }
}

std::unique_ptr<Sprite> SpriteLoader::loadSprite(const ResourceFile* file)
{
	if (!m_textures.count(file)) 
    {
        Logging::log("Loaded texture");
        SDL_Surface* new_surf = file->getAsSurface();
        SDL_Texture* new_text = SpriteLoader::convertSurfaceToTexture(new_surf);
        m_textures[file] = new_text;
    }
    return std::make_unique<Sprite>(m_textures[file]);
}
