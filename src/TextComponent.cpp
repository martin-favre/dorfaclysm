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

  size_t lastLinebreak = 0;
  size_t indx = 0;
  mSprites.clear();
  while(indx != text.npos)
  {
    indx = text.find("\n", lastLinebreak);
    std::string subString = text.substr(lastLinebreak, indx-lastLinebreak);
    mSprites.emplace_back(std::move(SpriteLoader::getSpriteFromTextFast(subString, *mFont, mColor)));
    lastLinebreak = indx+1;

  }

  // for(; indx < text.length(); ++indx)
  // {
  //   if(text[indx] == '\n')
  //   {
  //     std::string subString = text.substr(lastLinebreak, text[indx-1]);
  //     mSprites.emplace_back(std::move(SpriteLoader::getSpriteFromTextFast(text, *mFont, mColor)));
  //     lastLinebreak = indx+1;
  //   }
  // }

  // if(indx != text.length()-1)
  // {
  //   std::string subString = text.substr(lastLinebreak, text.length()-1);
  //   mSprites.emplace_back(std::move(SpriteLoader::getSpriteFromTextFast(text, *mFont, mColor)));
  // }

  // mSprite = SpriteLoader::getSpriteFromTextFast(text, *mFont, mColor);
}

void TextComponent::setColor(const SDL_Color& color) { mColor = color; }

void TextComponent::render() {

  ASSERT(mFont.get(), "Received null Font ptr");
  ASSERT(mFont->getSdlFont(), "Received null TTF_Font ptr");
  int fontHeight = TTF_FontHeight(mFont->getSdlFont());
  int heightOffset = 0;
  for(const auto& sprite : mSprites)
  {
    if(sprite->getSdlTexture() != nullptr)
    {
      const auto& cameraPos = Camera::get().getPosition();
      const GameObject& gObj = owner();
      Vector2DInt pos{gObj.getPosition()};
      if (mCameraAsReference) {
        pos += cameraPos;
      }
      pos.y += heightOffset;
      GraphicsManager::renderTexture(*sprite, pos, gObj.getScale(),
                                    gObj.getRotation(), mCentered, mFlip);
      heightOffset += fontHeight;
    }
  }

  // if (mSprite->getSdlTexture() == nullptr) return;
}