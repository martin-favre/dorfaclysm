#include "SpriteComponent.h"

#include <string>

#include "GameObject.h"
#include "GraphicsManager.h"
#include "Rect.h"
#include "Sprite.h"
#include "SpriteLoader.h"
#include "Vector2D.h"

SpriteComponent::SpriteComponent(GameObject &owner, std::unique_ptr<Sprite> && sprite)
    : Component(owner), mSprite(std::move(sprite)) {}

void SpriteComponent::render() {
  if (mSprite) {
    GraphicsManager::renderTexture(*mSprite, owner().getPosition(),
                                   owner().getScale(), owner().getRotation(),
                                   mCentered, mFlip);
  }
}