#include "TextComponent.h"

#include "Camera.h"
#include "Font.h"
#include "GameObject.h"
#include "GraphicsManager.h"
#include "SpriteLoader.h"
#include "Vector2DInt.h"

TextComponent::TextComponent(GameObject& owner, const std::string& pathToFont,
                             int size)
    : Component(owner),
      mFontSource(pathToFont),
      mFont(SpriteLoader::loadFont(pathToFont, size)) {}

void TextComponent::setFontSize(int size) {
  std::scoped_lock lock(mMutex);
  mFont = SpriteLoader::loadFont(mFontSource, size);
}

std::string TextComponent::getText() { return mText; }

void TextComponent::setText(const std::string& text) {
  if (text != mText) {
    std::scoped_lock lock(mMutex);
    size_t lastLinebreak = 0;
    size_t indx = 0;
    mRequestedSprites.clear();
    while (indx != text.npos) {
      indx = text.find("\n", lastLinebreak);
      std::string subString = text.substr(lastLinebreak, indx - lastLinebreak);
      if (subString != "") {
        mRequestedSprites.emplace_back(
            SpriteLoader::getSpriteFromTextFast(subString, *mFont, mColor));
      }
      lastLinebreak = indx + 1;
    }
    mText = text;
  }
}

void TextComponent::setColor(const SDL_Color& color) {
  std::scoped_lock lock(mMutex);
  mColor = color;
}

void TextComponent::teardown() { std::scoped_lock lock(mMutex); }

void TextComponent::render() {
  ASSERT(mFont.get(), "Received null Font ptr");
  ASSERT(mFont->getSdlFont(), "Received null TTF_Font ptr");
  std::scoped_lock lock(mMutex);
  if (mRequestedSprites.size()) {
    mSprites.clear();
    for (auto& sprite : mRequestedSprites) {
      mSprites.emplace_back(std::move(sprite));
    }
    mRequestedSprites.clear();
  }

  int fontHeight = TTF_FontHeight(mFont->getSdlFont());
  int heightOffset = 0;
  for (const auto& sprite : mSprites) {
    if (sprite->getSdlTexture() != nullptr) {
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
}