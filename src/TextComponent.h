#pragma once
#include <SDL_ttf.h>

#include <memory>
#include <string>
#include <vector>
#include "Component.h"
#include <mutex>
#include "Font.h"

class Sprite;
class GameObject;
class TextComponent : public Component {
 public:
  TextComponent(GameObject& owner, const std::string& pathToFont,
                int size = 16);
  TextComponent(GameObject& owner, const SerializedObj& serObj);
  SerializedObj serialize() const override;
  void setFontSize(int);
  std::string getText();
  void setText(const std::string&);
  void setColor(const SDL_Color& color);
  void updateText();
  void teardown() override;
  void render() override;
  void setCameraAsReference(bool useCamera) {mCameraAsReference = useCamera;}
  static std::string getTypeString();
 private:
  
  std::string mText;
  std::vector<std::unique_ptr<Sprite>> mRequestedSprites;
  std::vector<std::unique_ptr<Sprite>> mSprites;
  std::string mFontSource;
  int mSize;
  std::unique_ptr<Font> mFont;
  bool mCentered{false};
  bool mCameraAsReference{false};
  SDL_RendererFlip mFlip{SDL_FLIP_NONE};
  SDL_Color mColor{255, 255, 255, 255};
  std::mutex mMutex;
};