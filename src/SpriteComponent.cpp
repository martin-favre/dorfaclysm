#include "SpriteComponent.h"
#include "Vector2D.h"
#include "GraphicsManager.h"
#include "Rect.h"
#include "Sprite.h"
#include "SpriteLoader.h"
#include "GameObject.h"
#include <string>

SpriteComponent::SpriteComponent(GameObject &owner, const std::string &path) : Component(owner), mSprite(SpriteLoader::loadSprite(path)){}
SpriteComponent::SpriteComponent(GameObject &owner, const std::string &path, const Rect &area) : Component(owner), mSprite(SpriteLoader::loadSprite(path, area))
{

}
void SpriteComponent::loadSpriteFromImage(const std::string &path)
{
	mSprite = SpriteLoader::loadSprite(path);
}

void SpriteComponent::loadSpriteFromImage(const std::string &path, const Rect &area)
{
	mSprite = SpriteLoader::loadSprite(path, area);
}

void SpriteComponent::render()
{
	if (mSprite)
	{
		GraphicsManager::renderTexture(
			*mSprite,
			owner().getPosition(),
			owner().getScale(),
			owner().getRotation(),
			mCentered,
			mFlip);
	}
}