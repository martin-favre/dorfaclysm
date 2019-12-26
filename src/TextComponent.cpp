#include "TextComponent.h"

#include "Font.h"
#include "GameObject.h"
#include "GraphicsManager.h"
#include "SpriteLoader.h"

TextComponent::TextComponent(GameObject& owner, const std::string& pathToFont,
                             int size)
    : Component(owner),
      mFontSource(pathToFont),
      mFont(SpriteLoader::loadFont(pathToFont, size)) {}

void TextComponent::setFontSize(int size) {
  mFont = SpriteLoader::loadFont(mFontSource, size);
}

std::string TextComponent::getText() { return mText; }

void TextComponent::setText(const std::string& text) {
  mText = text;
  mSprite = SpriteLoader::getSpriteFromTextFast(text, *mFont, mColor);
}

void TextComponent::setColor(const SDL_Color& color) { mColor = color; }

void TextComponent::render() {
  if (mSprite->getSdlTexture() == nullptr) return;
  GraphicsManager::renderTexture(*mSprite, owner().getPosition(),
                                 owner().getScale(), mAngle, mCentered, mFlip);
}