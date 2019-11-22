#pragma once
#include <string>
#include <SDL.h>
#include "Vector2D.h"
#include "Helpers.h"
class Sprite {
public:
	Sprite(SDL_Texture * new_text);
	Sprite(SDL_Texture * new_text, bool isFont);
	Sprite(SDL_Texture * new_text, const SDL_Rect &);
	~Sprite();
	SDL_Texture * get_sdl_texture()const;
	const SDL_Rect * get_sdl_rect()const;
private:
	SDL_Texture * mTexture{nullptr};
	SDL_Rect mRect{0,0,0,0}; 
	const bool mIsFont{false};
};