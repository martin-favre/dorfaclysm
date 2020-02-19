#pragma once
#include "Component.h"
#include "Timer.h"
class TextComponent;

class FpsCounter : public Component {
 public:
  FpsCounter(GameObject& gObj);
  void setup() override;
  void update() override;
  void render() override;
 private:
  Timer mUpdateTimer;
  Timer mRenderTimer;
  int64_t mMsPerRender{0};
  TextComponent* mText{nullptr};
};