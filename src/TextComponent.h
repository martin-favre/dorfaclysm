#pragma once
#include <SDL_ttf.h>

#include <memory>
#include <string>

#include "Component.h"
#include "Vector2D.h"

class Font;
class Sprite;
class GameObject;
class TextComponent : public Component {
 public:
  TextComponent(GameObject& owner, const std::string& pathToFont,
                int size = 16);
  ~TextComponent() = default;
  void setFontSize(int);
  std::string getText();
  void setText(const std::string&);
  void setColor(const SDL_Color& color);
  void render() final override;

 private:
  std::string mText;
  std::unique_ptr<Sprite> mSprite;
  std::string mFontSource;
  std::unique_ptr<Font> mFont;
  Vector2D mScale{1, 1};
  double mAngle{0};
  bool mCentered{false};
  SDL_RendererFlip mFlip{SDL_FLIP_NONE};
  SDL_Color mColor{255, 255, 255, 1};
};