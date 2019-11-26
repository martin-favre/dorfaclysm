#pragma once
#include <SDL.h>
#include <SDL_image.h>
#include "Component.h"

class Rect;
class Sprite;
class GameObject;
class SpriteComponent : public Component
{
public:
	SpriteComponent(GameObject &owner) : Component(owner) {}
	void loadSpriteFromImage(const std::string& path);
	void loadSpriteFromImage(const std::string& path, const Rect &);
	void render() override;

private:
	std::unique_ptr<Sprite> mSprite;
	double mAngle = 0;
	bool mCentered = false;
	SDL_RendererFlip mFlip = SDL_FLIP_NONE;
};
