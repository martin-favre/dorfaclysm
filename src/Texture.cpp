#include "Texture.h"
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>

class Internals
{
public:
	SDL_Texture* m_texture;
};


Texture::Texture():m_internals(std::make_unique<Internals>())
{

}
