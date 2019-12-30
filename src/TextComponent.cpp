#include "TextComponent.h"

#include "Camera.h"
#include "Font.h"
#include "GameObject.h"
#include "GraphicsManager.h"
#include "SpriteLoader.h"
#include "Vector2DInt.h"
// Probably gonna use this eventually
// Vector2DInt getTextSize(const std::string& text, Font& font) {
//   int w = 0;
//   int h = 0;
//   int success = TTF_SizeText(font.getSdlFont(), text.c_str(), &w, &h);
//   ASSERT(success != -1, "TTF_SizeText failed " +
//   std::string(TTF_GetError())); return Vector2DInt{w, h};
// }

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
  const auto& cameraPos = Camera::get().getPosition();
  const GameObject& gObj = owner();
  Vector2DInt pos{gObj.getPosition()};
  if (mCameraAsReference) {
    pos += cameraPos;
  }
  GraphicsManager::renderTexture(*mSprite, pos, gObj.getScale(),
                                 gObj.getRotation(), mCentered, mFlip);
}