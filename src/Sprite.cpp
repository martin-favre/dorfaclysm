#include "Sprite.h"
#include <SDL_image.h>

Sprite::Sprite(SDL_Texture *newText) : Sprite(newText, false)
{
}

Sprite::Sprite(SDL_Texture *newText, bool isFont) : mTexture(newText), mIsFont(isFont)
{
	int width = 0;
	int height = 0;
	SDL_QueryTexture(mTexture, NULL, NULL, &width, &height);
	mRect = {0, 0, width, height};
}

Sprite::~Sprite()
{
	if (mIsFont)
	{
		// Since text textures can't be cached
		// We have to close the texture
		ASSERT(mTexture, "Is not supposed to be null");
		SDL_DestroyTexture(mTexture);
	}
}

Sprite::Sprite(SDL_Texture *newText, const SDL_Rect &newRect) : mTexture(newText),
																mRect(newRect) {}

SDL_Texture *Sprite::get_sdl_texture() const
{
	return mTexture;
}

const SDL_Rect *Sprite::get_sdl_rect() const
{
	return &mRect;
}
