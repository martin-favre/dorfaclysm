#include "textcomponent.hpp"
#include "SpriteLoader.h"
TextComponent::TextComponent(GameObject * owner): Component(owner){
}

TextComponent::~TextComponent() {
}

void TextComponent::initialize(const std::string& pathToFont, int size)
{
	ASSERT(file, "Trying to initialize Textcomponent with null resourcefile");
	mFontSource = file;
	mFont = SpriteLoader::loadFont(pathToFont, size);
}

void TextComponent::setFontSize(int size) {
	mFont = mFontSource->getAsFont(size);
	ASSERT(mFont, "Could not load font ");
	setText(mText);
}

std::string TextComponent::getText() {
	return mText;
}

void TextComponent::setText(const std::string & text) {
	mSprite = SpriteLoader::getSpriteFromText(mText, mFont, mColor);
	ASSERT(mSprite.getSdlTexture(), "Could not set text " + std::string(SDL_GetError()));
}

void TextComponent::setColor(const SDL_Color& color) {
	mColor = color;
}

void TextComponent::render() {
	if (mSprite->getSdlTexture() == nullptr) return;
	GraphicsManager::render_texture(
	    *mSprite,
	    owner()->get_position(),
	    mScale,
	    mAngle,
	    mCentered,
	    mFlip
	);
}