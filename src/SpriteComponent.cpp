#include "spritecomponent.h"
#include "Vector2D.h"
#include "GraphicsManager.h"
#include "Rect.h"
#include "Sprite.h"
#include "ResourceFile.h"
#include "SpriteLoader.h"
#include "GameObject"
void SpriteComponent::loadSpriteFromImage(const std::string& path)
{
	mSprite = SpriteLoader::loadSprite(path);
}

void SpriteComponent::loadSpriteFromImage(const std::string& path, const Rect & rect)
{
	mSprite = SpriteLoader::loadSprite(path, rect);
}

void SpriteComponent::render() {
	if (mSprite)
	{
		GraphicsManager::renderTexture(
			*mSprite,
			owner().get_position(),
			owner().get_scale(),
			owner().get_rotation(),
			mCentered,
			mFlip
		);
	}
}