#include "TextComponent.h"
#include "SpriteLoader.h"
#include "GraphicsManager.h"
#include "Font.h"
#include "GameObject.h"

TextComponent::TextComponent(GameObject& owner): Component(owner){
}

TextComponent::~TextComponent() {
}

void TextComponent::initialize(const std::string& pathToFont, int size)
{
	mFontSource = pathToFont;
	mFont = SpriteLoader::loadFont(pathToFont, size);
}

void TextComponent::setFontSize(int size) {
	mFont = SpriteLoader::loadFont(mFontSource, size);
}

std::string TextComponent::getText() {
	return mText;
}

void TextComponent::setText(const std::string & text) {
	mText = text;
	mSprite = SpriteLoader::getSpriteFromText(text, *mFont, mColor);
}

void TextComponent::setColor(const SDL_Color& color) {
	mColor = color;
}

void TextComponent::render() {
	if (mSprite->getSdlTexture() == nullptr) return;
	GraphicsManager::renderTexture(
	    *mSprite,
	    owner().getPosition(),
	    mScale,
	    mAngle,
	    mCentered,
	    mFlip
	);
}