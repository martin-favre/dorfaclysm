#pragma once
#include <string>
#include <SDL.h>
#include "Vector2D.h"
#include "Helpers.h"
class Sprite {
public:
	Sprite(SDL_Texture * newText, bool isFont = false);
	Sprite(SDL_Texture * newText, const Rect &, bool isFont = false);
	~Sprite();
	SDL_Texture * getSdlTexture()const;
	const Rect& getSdlRect()const;
private:
	SDL_Texture * mTexture{nullptr};
	Rect mRect; 
	const bool mIsFont{false};
};